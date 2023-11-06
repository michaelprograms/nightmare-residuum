#include <access.h>
#include <driver/function.h>

private nosave mapping __Group, __Read, __Write;
private nosave object __Unguarded;

string query_file_privs (string filename);

private mapping load_config (string path) {
    mapping result = ([ ]);
    string file, *lines, key, value;
    int num;

    if (file = read_file(path)) {
        lines = explode(file, "\n");
        num = sizeof(lines);
        for (int i = 0; i < num; i ++) {
            if (!lines[i] || lines[i] == "" || lines[i][0] == '#') continue;
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

// int securep (mixed who) {
//     mixed tmp = who;
//     if (!who) who = previous_object();
//     if (!stringp(who)) who = who->query_key_name();
//     if (!who || who == "") who = base_name(tmp);
//     if (member_group(who, "SECURE")) return 1;
//     else return 0;
// }

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
    if (!stringp(file)) {
        error("Bad argument 3 to access->query_allowed");
    }
    if (!stringp(mode)) {
        error("Bad argument 4 to access->query_allowed");
    }

    if (mode == "socket") {
        // only D_IPC allowed to create sockets
        return regexp(file_name(caller), "^/secure/daemon/ipc");
    }

    // attempt to match the target file path to __Read path permissions
    pathPrivs = match_path(__Read, file);

    // access check passes due to root privilege of matched file path being ALL
    if (sizeof(pathPrivs) && pathPrivs[0] == ACCESS_ALL) {
        return 1;
    }

    // debug_message("! D_ACCESS->query_allowed with " + identify(caller) + ", "+identify(pathPrivs)+", "+file+" fn: "+fn);

    // caller utilized unguarded(function)
    if (__Unguarded == caller) {
        tmp = base_name(caller);
        // debug_message("! D_ACCESS->query_allowed unguarded == caller: "+identify(caller)+" "+tmp);
        // access check passes due to caller requesting valid save path
        if (regexp(tmp, "^"+STD_CHARACTER[0..<3]) && D_CHARACTER->query_valid_save_path(caller->query_key_name(), file)) {
            return 1;
        } else if (regexp(tmp, "^/std/npc/pet") && D_CHARACTER->query_valid_save_path(caller->query_owner_name(), file)) {
            return 1;
        } else if (regexp(tmp, "^"+STD_USER[0..<3]) && D_ACCOUNT->query_save_path(caller->query_key_name()) == file) {
            return 1;
        } else if (regexp(tmp, "^"+D_LOG) && regexp(file, "^/log/")) {
            return 1;
        } else {
            // set caller as the stack
            i = sizeof(stack = ({ caller }));
        }
    } else {
        // set all previous objects and caller as the stack
        i = sizeof(stack = previous_object(-1) + ({ caller }));
    }

    // debug_message("! D_ACCESS->query_allowed stack is: "+identify(stack));

    // go through the stack from the caller down thru possible previous objects
    while (i--) {
        // skip invalid stack entry
        if (!stack[i]) continue;

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
        switch(path[0]) {
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
                result = sizeof(path) > 1 ? lower_case(path[1]) : 0;
                break;
            case "domain":
                result = sizeof(path) > 1 ? capitalize(lower_case(path[1])) : 0;
                break;
            default: result =  0;
        }
    }
    return result;
}

mixed unguarded (function f) {
    object previous_unguarded;
    string err;
    mixed value;

    if (!f || !functionp(f)) {
        error("Bad argument 1 to access->unguarded");
    }

    if (
        !regexp(base_name(previous_object(0)), "^/secure/sefun/") ||
        (functionp(f) & FP_OWNER_DESTED)
    ) {
        error("Bad previous_object to access->unguarded");
    }
    previous_unguarded = __Unguarded;
    __Unguarded = previous_object(1);
    err = catch (value = evaluate(f));
    __Unguarded = previous_unguarded;
    if (err) {
        error(err);
    }
    return value;
}