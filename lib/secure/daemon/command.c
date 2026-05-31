inherit M_CLEAN;

nosave private string *__Paths = ({ });
nosave private mapping __Abilities = ([ ]);
nosave private mapping __Commands = ([ ]);
nosave private mapping __Verbs = ([ ]);

string *query_paths () {
    return __Paths;
}

/* ----- querying single ----- */

string query_ability (string ability) {
    if (__Abilities[ability]) {
        return __Abilities[ability][0];
    }
    return 0;
}
string query_command (string command) {
    if (__Commands[command]) {
        // hide immortal commands from non-immortals
        if (!sizeof(filter(previous_object(-1), (: $1->query_immortal() :))) && regexp(__Commands[command][0], "/immortal")) {
            return 0;
        }
        return __Commands[command][0];
    }
    return 0;
}
string query_verb (string verb) {
    if (__Verbs[verb] && __Verbs[verb][0]) {
        // verbs have to be loaded for parsing rules to apply
        load_object(__Verbs[verb][0] + "/" + verb + ".c");
    }
    return __Verbs[verb] ? __Verbs[verb][0] : 0;
}

/* ----- querying all ----- */

private varargs string *query_registry (mapping registry, string str) {
    string *items, *tmp;
    int i;

    if (!str) {
        return keys(registry);
    }
    i = sizeof(items = keys(registry));
    tmp = ({ });
    while (i--) {
        if (member_array(str, registry[items[i]]) != -1) {
            tmp += ({ items[i] });
        }
    }
    return tmp;
}
varargs string *query_abilities (string str) {
    return query_registry(__Abilities, str);
}
varargs string *query_commands (string str) {
    return query_registry(__Commands, str);
}
varargs string *query_verbs (string str) {
    return query_registry(__Verbs, str);
}

/* ----- scanning ----- */

private void scan (string *paths, string type) {
    string cmd;
    mapping target = ([ "ability": __Abilities, "command": __Commands, "verb": __Verbs ])[type];
    int load = (type == "ability" || type == "verb");

    foreach (string path in paths) {
        foreach (string file in get_dir(path + "/*.c")) {
            cmd = file[0..<3];
            if (!arrayp(target[cmd])) {
                target[cmd] = ({ });
            }
            target[cmd] += ({ path });
            if (load) {
                load_object(path + "/" + file);
            }
        }
        __Paths = distinct_array(__Paths + ({ path }));
    }
}

void scan_all_paths () {
    __Paths = ({ });
    __Abilities = ([ ]);
    __Commands = ([ ]);
    __Verbs = ([ ]);

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

/* ----- applies ----- */

void create () {
    set_no_clean(1);
    scan_all_paths();
}