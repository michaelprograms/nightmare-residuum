#include <access.h>
#include <driver/function.h>

private nosave mapping __Group, __Read, __Write;

string query_file_privs (string filename);

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

void create () {
    __Group = load_config("/secure/etc/group.cfg");
    __Read = load_config("/secure/etc/read.cfg");
    __Write = load_config("/secure/etc/write.cfg");
}

// -----------------------------------------------------------------------------

// The main function used to check if a caller can perform fn on file in mode.
int query_allowed (object caller, string fn, string file, string mode) {
    string *pathPrivs, *privs, priv, tmp;
    object *stack;
    int i;

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

    if (mode == "socket") {
        // only D_IPC or HTTP module allowed to create sockets
        return file_name(caller) == "/secure/daemon/ipc" ||
            inherits("/secure/module/http.c", caller);
    }

    // attempt to match the target file path to __Read path permissions
    pathPrivs = match_path(__Read, file);

    // access check passes due to root privilege of matched file path being ALL
    if (sizeof(pathPrivs) && pathPrivs[0] == ACCESS_ALL) {
        return 1;
    }

    // debug_message("! D_ACCESS->query_allowed with " + identify(caller) + ", "+identify(pathPrivs)+", "+file+" fn: "+fn);

    // set all previous objects and caller as the stack
    i = sizeof(stack = previous_object(-1) + ({ caller }));

    // debug_message("! D_ACCESS->query_allowed stack is: "+identify(stack));

    // go through the stack from the caller down thru possible previous objects
    while (i--) {
        // skip invalid stack entry
        if (!stack[i]) {
            continue;
        }

        // skip if stack entry is member of the access system
        tmp = file_name(stack[i]);
        if (stack[i] == this_object() || tmp == MASTER || tmp == SEFUN) {
            continue;
        }

        // access check fails due to no privs found for stack entry
        if (!(priv = query_privs(stack[i]))) {
            return 0;
        }

        // skip if trying to read with no read privilege
        if (!pathPrivs && mode == "read") {
            continue;
        }

        // prepare list of stack entry's privilege groups
        privs = explode(priv, ":"); // @TODO when does this receive a : separated list?

        // skip stack entry without SECURE priv
        if (member_array(ACCESS_SECURE, privs) != -1) {
            continue;
        }

        // skip stack entry with privs containing that of target file
        if (member_array(query_file_privs(file), privs) != -1) {
            continue;
        }

        // access check fails due to trying to write with no read privilege
        if (!pathPrivs && mode == "write") {
            return 0;
        }

        // skip stack entry if privs and pathPrivs are identical arrays
        if (sizeof(privs & pathPrivs)) {
            continue;
        }

        // access check fails due to reaching a stack entry without privilege
        return 0;
    }
    // debug_message("! D_ACCESS->query_allowed returning 1\n--------");

    // access check has cleared the stack with privilege
    return 1;
}

// master apply privs_file calls this function
string query_file_privs (string filename) {
    string *path, result = 0;
    if (sizeof(path = explode(filename, "/")) > 0) {
        switch (path[0]) {
            case "cmd":
                result = ACCESS_CMD; // @TODO necessary?
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
            default: result =  0;
        }
    }
    return result;
}