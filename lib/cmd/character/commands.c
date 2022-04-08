void command (string input, mapping flags) {
    object tc = this_character(), target = tc;
    mapping actions = ([]);
    string *border, subtitle, *body = ({});
    mapping header;

    if (flags["r"]) {
        D_COMMAND->scan_all();

        subtitle = "Rescanned";
        header = ([
            "items": ({ "Commands and verbs rescanned." }),
            "columns": 1,
            "align": "center",
        ]);
    }

    foreach (string path in D_COMMAND->query_paths()) {
        string type = split_path(path)[<1];
        if (!actions[type]) actions[type] = ({});
        if (type == "immortal" || type == "character") {
            foreach (string action in D_COMMAND->query_commands(path)) {
                actions[type] += ({ action });
            }
        } else if (type == "verb") {
            foreach (string action in D_COMMAND->query_verbs(path)) {
                actions[type] += ({ action });
            }
        }
    }
    foreach (string type in keys(actions)) {
        mapping b = ([
            "header": capitalize(type),
            "columns": 4,
            "items": ({ }),
        ]);
        foreach(string a in actions[type]) {
            b["items"] += ({ a });
        }
        body += ({ b });
    }

    border = format_border(([
        "title": "COMMANDS",
        "subtitle": subtitle,
        "header": header,
        "body": body,
    ]));
    foreach (string line in border) {
        message("system", line + "\n", tc);
    }
}