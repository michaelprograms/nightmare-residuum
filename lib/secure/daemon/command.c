inherit M_CLEAN;

nosave private string *__Paths = ({});
nosave private mapping __Commands = ([]);
nosave private mapping __Verbs = ([]);

void scan (string *paths, string type);

string *query_paths () { return __Paths; }
mapping query_debug_commands () { return __Commands; }
mapping query_debug_verbs () { return __Verbs; }

void create () {
    set_no_clean(1);
    __Paths = ({});
    __Commands = ([]);
    __Verbs = ([]);
    scan(({
        "/cmd/character",
        "/cmd/immortal",
        "/secure/cmd/character",
        "/secure/cmd/immortal",
    }), "command");
    scan(({
        "/cmd/verb",
        "/secure/cmd/verb",
    }), "verb");
}

// @TODO used by help
varargs string *query_commands (string str) {
    string *cmds, *tmp;
    int i;

    if (!str) return keys(__Commands);
    i = sizeof(cmds = keys(__Commands));
    tmp = ({});
    while (i--) {
        if (member_array(str, __Commands[cmds[i]]) != -1) {
            tmp += ({ cmds[i] });
        }
    }
    return tmp;
}
varargs string *query_verbs (string str) {
    string *verbs, *tmp;
    int i;

    if (!str) return keys(__Verbs);
    i = sizeof(verbs = keys(__Verbs));
    tmp = ({});
    while (i--) {
        if (member_array(str, __Verbs[verbs[i]]) != -1) {
            tmp += ({ verbs[i] });
        }
    }
    return tmp;
}

// string query_action (string str) {
//     if (__Verbs[str] && __Verbs[str][0]) {
//         // verbs have to be loaded for parsing rules to apply
//         if (!catch (call_other(__Verbs[str][0]+"/"+str+".c", "???"))) {
//             write(__Verbs[str][0]+"/"+str+".c: Ok\n");
//         } else write(__Verbs[str][0]+"/"+str+".c: not loaded?\n");
//         return 1;
//     } else if (__Commands[str]) {
//         return 1;
//     } else return 0;
// }
// varargs string *query_actions (string str) {
//     string *actions, *tmp;
//     int i;

//     if (!str) return keys(__Commands) + keys(__Actions);

// }

string query_command (string command) {
    return __Commands[command] ? __Commands[command][0] : 0;
}
string query_verb (string verb) {
    if (__Verbs[verb] && __Verbs[verb][0]) {
        // verbs have to be loaded for parsing rules to apply
        call_other(__Verbs[verb][0]+"/"+verb+".c", "???");
    }
    return __Verbs[verb] ? __Verbs[verb][0] : 0;
}

void scan (string *paths, string type) {
    if (stringp(paths)) paths = ({ paths });
    else if (!arrayp(paths)) return;
    foreach (string path in paths) {
        if (file_size(path) != -2) continue;
        foreach (string file in get_dir(path + "/*.c")) {
            string cmd = file[0..<3];
            if (type == "command") {
                if (!arrayp(__Commands[cmd])) __Commands[cmd] = ({ });
                __Commands[cmd] += ({ path });
            } else if (type == "verb") {
                if (!arrayp(__Verbs[cmd])) __Verbs[cmd] = ({ });
                __Verbs[cmd] += ({ path });
                call_other(path + "/" + file, "???");
            }
        }
        __Paths = distinct_array(__Paths + ({ path }));
        debug_message("!!! __Paths is now "+identify(__Paths));
    }
}