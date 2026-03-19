#include <access.h>
#include <driver/function.h>

private nosave mapping __Read, __Write;
nosave private int __Debug = 0;

/**
 * Parses a key-value access config file into a mapping.
 * Each non-comment line must be in the format: (path) PRIV or (path) PRIV:PRIV2
 *
 * @param {string} path - absolute path to the config file
 * @returns {mapping} parsed config — keys are paths, values are string arrays of privileges
 */
private mapping load_config (string path) {
    mapping result = ([ ]);
    string file, *lines, key, value;
    int num;

    if (file = read_file(path)) {
        lines = explode(file, "\n");
        num = sizeof(lines);
        for (int i = 0; i < num; i ++) {
            if (!lines[i] || lines[i] == "" || lines[i][0] == '#') {
                continue;
            }
            if (sscanf(lines[i], "(%s) %s", key, value) == 2) {
                result[key] = explode(value, ":");
            }
        }
    }
    return result;
}

/**
 * Loads the access config files on daemon startup.
 */
void create () {
    __Read = load_config("/secure/etc/read.cfg");
    __Write = load_config("/secure/etc/write.cfg");
}

/**
 * Enables or disables debug logging for access checks.
 * When enabled, each stack entry evaluation emits a debug_message to the driver log.
 *
 * @param {int} val - 1 to enable debug output, 0 to disable
 */
void set_debug (int val) {
    if (!intp(val)) {
        error("Bad argument 1 to access->set_debug");
    }
    __Debug = val;
}

/**
 * Returns the privilege class that a file path belongs to.
 * Called by master.c's privs_file apply to assign object identity when a
 * file is first compiled. This determines what an object *is*, not what it
 * can *access* — see query_allowed for runtime access control.
 *
 * Realm files return the lowercased realm name. Domain files return the
 * capitalized domain name. All other paths map to an ACCESS_* constant.
 *
 * @param {string} filename - absolute path to the file (leading slash optional)
 * @returns {string} the privilege class string, or 0 if path is unrecognized
 */
string query_file_privs (string filename) {
    string *path, result = 0;
    if (sizeof(path = explode(filename, "/")) > 0) {
        switch (path[0]) {
            case "cmd":
                result = ACCESS_CMD;
                break;
            case "etc":
                result = ACCESS_ALL;
                break;
            case "daemon":
                result = ACCESS_MUDLIB;
                break;
            case "save":
                result = ACCESS_SECURE;
                break;
            case "std":
                result = ACCESS_ASSIST;
                break;
            case "secure":
                result = ACCESS_SECURE;
                break;
            case "realm":
                if (sizeof(path) > 1) {
                    result = lower_case(path[1]);
                }
                break;
            case "domain":
                if (sizeof(path) > 1) {
                    result = capitalize(lower_case(path[1]));
                }
                break;
            default:
                result = 0;
        }
    }
    return result;
}

/**
 * Emits a prefixed debug_message to the driver log if debug mode is enabled.
 * Called by check_stack_entry at each decision point.
 *
 * @param {string} msg - the message to emit (will be prefixed with "ACCESS_D: ")
 */
void print_debug_message (string msg) {
    if (__Debug) {
        debug_message("ACCESS_D: " + msg);
    }
}

/**
 * Evaluates a single call stack entry against the access requirements for a
 * file operation. Called once per stack entry by query_allowed.
 *
 * Returns 1 if this entry clears (access granted for this entry).
 * Returns 0 if this entry fails (caller should be denied access).
 *
 * Checks in order: internal object skip → no privilege → open read path →
 * SECURE bypass → file class match → closed write path → privilege intersection.
 *
 * @param {object} ob              - the call stack entry being evaluated
 * @param {string *|int} requiredPrivs - privileges required by the path config, or 0 if path has no config entry
 * @param {string} mode            - "read" or "write"
 * @param {string} file            - the target file path
 * @returns {int} 1 if entry clears, 0 if access denied
 */
private int check_stack_entry (object ob, string *requiredPrivs, string mode, string file) {
    string entryPriv, entryFile, filePriv;
    string *entryPrivs;

    // Skip internal security objects — they are always trusted
    entryFile = file_name(ob);
    if (ob == this_object() || entryFile == MASTER || entryFile == SEFUN) {
        print_debug_message(entryFile + " (internal object)");
        return 1;
    }

    // Objects without any privilege are never allowed
    if (!(entryPriv = query_privs(ob))) {
        print_debug_message(entryFile + " (no privileges)");
        return 0;
    }

    // No read.cfg entry matched this path — any privileged object may read
    if (!requiredPrivs && mode == "read") {
        print_debug_message(entryFile + " (open read path)");
        return 1;
    }

    entryPrivs = explode(entryPriv, ":");

    // SECURE objects bypass all path restrictions
    if (member_array(ACCESS_SECURE, entryPrivs) != -1) {
        print_debug_message(entryFile + " (SECURE privilege)");
        return 1;
    }

    // Objects whose privilege class matches the file's privilege class are allowed
    filePriv = query_file_privs(file);
    if (member_array(filePriv, entryPrivs) != -1) {
        print_debug_message(entryFile + " (file class match: " + filePriv + ")");
        return 1;
    }

    // Unprotected write paths — deny by default
    if (!requiredPrivs && mode == "write") {
        print_debug_message(entryFile + " (no write config for path)");
        return 0;
    }

    // Object holds at least one privilege listed in the path's required set
    if (sizeof(entryPrivs & requiredPrivs)) {
        print_debug_message(entryFile + " (privilege match: " + implode(entryPrivs & requiredPrivs, ":") + ")");
        return 1;
    }

    print_debug_message(entryFile + " (needs: " + implode(requiredPrivs, ":") + ", has: " + entryPriv + ")");
    return 0;
}

/**
 * The main runtime access check. Called by master.c's valid_read and
 * valid_write applies to determine whether a file operation is permitted.
 *
 * Paths marked ALL in the config are immediately allowed without stack
 * inspection. Otherwise, every object in the call stack must pass
 * check_stack_entry — if any single entry lacks sufficient privilege,
 * access is denied.
 *
 * Socket access bypasses the normal config check and is permitted only
 * for the object at /secure/daemon/ipc or any inheritor of the HTTP module.
 *
 * @param {object} caller    - the object initiating the file operation
 * @param {string} fn        - the efun being called (e.g., "read_file", "write_file")
 * @param {string|int} file  - the target file path, or 0 for socket mode
 * @param {string} mode      - "read", "write", or "socket"
 * @returns {int} 1 if access is allowed, 0 if denied
 */
int query_allowed (object caller, string fn, string file, string mode) {
    string *requiredPrivs;
    object *callStack;

    if (!objectp(caller)) {
        error("Bad argument 1 to access->query_allowed");
    }
    if (!stringp(fn)) {
        error("Bad argument 2 to access->query_allowed");
    }
    if (!stringp(file) && mode != "socket") {
        error("Bad argument 3 to access->query_allowed");
    }
    if (!stringp(mode)) {
        error("Bad argument 4 to access->query_allowed");
    }

    // Socket access is handled separately
    if (mode == "socket") {
        return file_name(caller) == "/secure/daemon/ipc" ||
            inherits("/secure/module/http.c", caller);
    }

    // Select required privileges for this path based on the operation mode
    requiredPrivs = mode == "write"
        ? match_path(__Write, file)
        : match_path(__Read, file);

    // Publicly accessible paths are immediately allowed without stack inspection
    if (sizeof(requiredPrivs) && requiredPrivs[0] == ACCESS_ALL) {
        return 1;
    }

    // Every object in the call stack must hold sufficient privilege
    callStack = ({ caller }) + previous_object(-1);
    foreach (object ob in callStack) {
        if (!ob) {
            continue;
        }
        if (!check_stack_entry(ob, requiredPrivs, mode, file)) {
            return 0;
        }
    }
    return 1;
}