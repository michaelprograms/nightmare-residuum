inherit M_CLEAN;

nosave private string *__Paths = ({});
nosave private mapping __Abilities = ([]);
nosave private mapping __Commands = ([]);
nosave private mapping __Verbs = ([]);

string *query_paths () { return __Paths; }
mapping query_debug_abiltiies () { return __Abilities; }
mapping query_debug_commands () { return __Commands; }
mapping query_debug_verbs () { return __Verbs; }
void scan (string *paths, string type);
void scan_all ();

void create () {
    set_no_clean(1);
    __Paths = ({});
    __Abilities = ([]);
    __Commands = ([]);
    __Verbs = ([]);
    scan_all();
}

// @TODO used by help
varargs string *query_abilities (string str) {
    string *abilities, *tmp;
    int i;

    if (!str) return keys(__Abilities);
    i = sizeof(abilities = keys(__Abilities));
    tmp = ({});
    while (i--) {
        if (member(str, __Abilities[abilities[i]]) != -1) {
            tmp += ({ abilities[i] });
        }
    }
    return tmp;
}
varargs string *query_commands (string str) {
    string *cmds, *tmp;
    int i;

    if (!str) return keys(__Commands);
    i = sizeof(cmds = keys(__Commands));
    tmp = ({});
    while (i--) {
        if (member(str, __Commands[cmds[i]]) != -1) {
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
        if (member(str, __Verbs[verbs[i]]) != -1) {
            tmp += ({ verbs[i] });
        }
    }
    return tmp;
}

string query_ability (string ability) {
    return __Abilities[ability] ? __Abilities[ability][0] : 0;
}
string query_command (string command) {
    if (__Commands[command]) {
        if (!previous_object()->query_character()->query_immortal() && sizeof(regexp(__Commands[command][0], "/immortal"))) return 0;
        return __Commands[command][0];
    } else return 0;
}
string query_verb (string verb) {
    if (__Verbs[verb] && __Verbs[verb][0]) {
        // verbs have to be loaded for parsing rules to apply
        load_object(__Verbs[verb][0] + "/" + verb + ".c");
    }
    return __Verbs[verb] ? __Verbs[verb][0] : 0;
}

void scan (string *paths, string type) {
    if (stringp(paths)) paths = ({ paths });
    else if (!pointerp(paths)) return;
    foreach (string path in paths) {
        if (file_size(path) != -2) continue;
        foreach (string file in get_dir(path + "/*.c")) {
            string cmd = file[0..<3];
            if (type == "ability") {
                if (!pointerp(__Abilities[cmd])) __Abilities[cmd] = ({ });
                __Abilities[cmd] += ({ path });
                // load all verb rules
                load_object(path + "/" + file);
            } else if (type == "command") {
                if (!pointerp(__Commands[cmd])) __Commands[cmd] = ({ });
                __Commands[cmd] += ({ path });
            } else if (type == "verb") {
                if (!pointerp(__Verbs[cmd])) __Verbs[cmd] = ({ });
                __Verbs[cmd] += ({ path });
                // load all verb rules
                load_object(path + "/" + file);
            }
        }
        __Paths = distinct_array(__Paths + ({ path }));
    }
}

void scan_all () {
    scan(({
        "/cmd/ability",
    }), "ability");
    scan(({
        "/cmd/character",
        "/cmd/immortal",
        "/secure/cmd/character",
        "/secure/cmd/immortal",
    }), "command");
    scan(({
        "/cmd/verb",
    }), "verb");
}