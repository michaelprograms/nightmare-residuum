inherit STD_COMMAND;

void create () {
    ::create();
    set_syntax("settings ([setting] [value])");
    set_help_text("The settings command is used to view your account's settings.");
}

void command (string input, mapping flags) {
    object tc = this_character(), target = tc, user;
    mixed *settings = ({ });
    string *list = ({ });
    mapping header;

    if (input && tc->query_immortal()) {
        if (find_character(input)) {
            target = find_character(input);
            input = 0;
        }
    }

    user = target->query_user();

    if (input) {
        string key, *split;
        mixed value;

        split = explode(input, " ");
        key = split[0];
        value = implode(split[1..], " ");

        switch (key) {
            case "width": case "lines":
                value = to_int(value);
                break;
            case "ansi": case "gmcp": case "screenreader":
                if (member_array(value, ({ "on", "off" })) == -1) {
                    message("action", "Invalid value '" + value +"' for setting '" + key + "'.", user);
                    return;
                }
                break;
        }
        header = ([
            "columns": 1,
            "align": "center",
        ]);
        if (member_array(key, keys(user->query_settings())) == -1) {
            header["items"] = ({ "Invalid setting: " + key });
        } else {
            user->set_setting(key, value);
            header["items"] = ({ "Setting %^BOLD%^" + key + "%^BOLD_OFF%^ to %^BOLD%^" + value + "%^BOLD_OFF%^" });
        }
    }

    foreach (string key,mixed value in user->query_settings()) {
        settings += ({ ({ key, value }) });
    }
    settings = sort_array(settings, (: strcmp($1[0], $2[0]) :));
    foreach (string *setting in settings) {
        list += ({ sprintf("%16s : %s", setting[0], ""+setting[1]) });
    }

    border(([
        "title": "SETTINGS",
        "subtitle": target->query_cap_name(),
        "header": header,
        "body": ([
            "items": list,
            "columns": 1,
        ]),
    ]));
}