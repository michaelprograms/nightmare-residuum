inherit STD_COMMAND;

void create () {
    set_syntax("commands");
    set_help_text("The commands command is used to view the list of possible actions your character can make.");
}

void command (string input, mapping flags) {
    object tc = this_character();
    mapping actions = ([ ]);
    string *border, subtitle;
    mixed *body = ({ });
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
        if (!actions[type]) actions[type] = ({ });
        if (type == "immortal" || type == "character") {
            foreach (string action in D_COMMAND->query_commands(path)) {
                actions[type] += ({ action });
            }
        } else if (type == "ability") {
            foreach (string action in D_COMMAND->query_abilities(path)) {
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
        foreach (string a in sort_array(actions[type], 1)) {
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