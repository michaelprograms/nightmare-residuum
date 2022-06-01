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
//     if (!who || who == "") who = object_name(tmp);
//     if (member_group(who, "SECURE")) return 1;
//     else return 0;
// }


// nomask private int check_user (object ob, string fun, string file, string mode) {
//     string nom;
//     int x;

//     if (!sscanf(file, "/realm/%s/%*s", nom)) return 0;
//     nom = user_path(nom)+"adm/access";
//     if (file_size(nom+".c") < 0) return 0;
//     catch(x = (int)call_other(nom, "query_allowed", ob, fun, file, mode));
//     return x;
// }
// nomask private int check_domain (object ob, string fun, string file, string mode) {
//     string nom;
//     int x;

//     if (!sscanf(file, "/domain/%s/%*s", nom)) return 0;
//     nom = "/domain/"+nom+"/adm/access";
//     if (file_size(nom+".c") < 0) return 0;
//     catch (x = (int)call_other(nom, "query_allowed", ob, fun, file, mode));
//     return x;
// }

// -----------------------------------------------------------------------------

string *match_path (mapping m, string path) {
    string match;
    int n = 0;
    foreach (string key in m_indices(m)) {
        if (strstr(key, path) > n) {
            n = strstr(key, path);
            match = key;
        }
    }
    return m[match];
}

// The main function used to check if a caller can perform fn on file in mode.
int query_allowed (object caller, string fn, string file, string mode) {
    string *pathPrivs, *privs, priv;
    object *stack;
    int i;

    return 1;

    if (!objectp(caller)) raise_error("Bad argument 1 to access->query_allowed");
    if (!stringp(fn)) raise_error("Bad argument 2 to access->query_allowed");
    if (!stringp(file)) raise_error("Bad argument 3 to access->query_allowed");
    if (!stringp(mode)) raise_error("Bad argument 4 to access->query_allowed");

    // attempt to match the target file path to __Read path permissions
    pathPrivs = match_path(__Read, file);

    // access check passes due to root privilege of matched file path being ALL
    if (pathPrivs && sizeof(pathPrivs) && pathPrivs[0] == ACCESS_ALL) return 1;

    // debug_message("! D_ACCESS->query_allowed with " + identify(caller) + ", "+identify(pathPrivs)+", "+file+" fn: "+fn);

    // caller utilized unguarded(function)
    if (__Unguarded == caller) {
        string tmp = object_name(caller);
        // debug_message("! D_ACCESS->query_allowed unguarded == caller: "+identify(caller)+" "+tmp);
        // access check passes due to caller requesting valid save path
        if (!strstr(tmp, "/std/character") && D_CHARACTER->query_valid_save_path(caller->query_key_name(), file)) {
            return 1;
        } else if (!strstr(tmp, "/std/account") && D_ACCOUNT->query_save_path(caller->query_key_name()) == file) {
            return 1;
        } else if (!strstr(tmp, "/daemon/log") && sizeof(regexp(({ file }), "^/log/"))) {
            return 1;
        } else {
            // set caller as the stack
            i = sizeof(stack = ({ caller }));
        }
    } else {
        // set all previous objects and caller as the stack
        i = sizeof(stack = caller_stack() + ({ caller }));
    }
    // @TODO ? else if (__Unguarded && object_name(caller) == SEFUN) {}

    // debug_message("! D_ACCESS->query_allowed stack is: "+identify(stack));

    // go through the stack from the caller down thru possible previous objects
    while (i--) {
        // skip invalid stack entry
        if (!stack[i]) continue;

        // skip if stack entry is member of the access system
        if (stack[i] == this_object() || program_name(stack[i])[0..<3] == MASTER || program_name(stack[i])[0..<3] == SEFUN) continue;

        // access check fails due to no privs found for stack entry
        if (!(priv = query_file_privs(program_name(stack[i])))) return 0;

        // skip if trying to read with no read privilege
        if (!pathPrivs && mode == "read") continue;

        // prepare list of stack entry's privilege groups
        privs = explode(priv, ":"); // @TODO when does this receive a : separated list?

        // skip stack entry without SECURE priv
        if (member(ACCESS_SECURE, privs) != -1) continue;

        // skip stack entry with privs containing that of target file
        if (member(query_file_privs(file), privs) != -1) continue;

        // access check fails due to trying to write with no read privilege
        if (!pathPrivs && mode == "write") return 0;
        // @TODO check_user really necessary? :/
        // { if (userp(stack[i]) && check_user(stack[i], fn, file, mode)) continue; else return 0; }

        // skip stack entry if privs and pathPrivs are identical arrays
        if (sizeof(privs & pathPrivs)) continue;

        // @TODO check_user and check_domain really necessary? :/
        // if (userp(stack[i]) && check_user(stack[i], fn, file, mode)) continue;
        // if (userp(stack[i]) && check_domain(stack[i], fn, file, mode)) continue;

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

mixed unguarded (closure f) {
    object previous_unguarded;
    string err;
    mixed value;

    debug_message("D_ACCESS->unguarded\n");
    if (!f || !closurep(f) || sizeof(regexp(({ object_name(previous_object(0)) }), "^\\/secure\\/sefun\\/"))) {
        // @LDMUD MIGRATION REMOVED: || (closurep(f) & FP_OWNER_DESTED)
        raise_error("Illegal unguarded.");
        return 0;
    }
    previous_unguarded = __Unguarded;
    __Unguarded = previous_object(1);
    err = catch (value = funcall(f));
    __Unguarded = previous_unguarded;
    if (err) raise_error(err);
    return value;
}