inherit STD_COMMAND;

void create () {
    set_syntax("time");
    set_help_text("The time command is used to view the current, local, server, and up time.");
}

void command (string input, mapping flags) {
    string *border, localtime, localdate;
    mapping a;

    a = D_ASTRONOMY->query_astronomy_from_room(environment(this_character()));
    localtime = D_ASTRONOMY->query_localtime(a) + " of " + a["HOURS_PER_DAY"] + ":00";
    localdate = D_ASTRONOMY->query_localdate(a);

    border = format_border(([
        "title": "TIME",
        "body": ({
            a ? ([
                "header": ({ "Local Time", "Local Date", }),
                "items": ({
                    localtime,
                    localdate,
                }),
                "columns": 2,
                "align": "center",
            ]) : ([
                "items": ({
                    "No local astronomy found",
                }),
                "columns": 1,
                "align": "center",
            ]),
        }),
        "footer": ([
            "header": ({ "Server Time", "Up Time" }),
            "items": ({
                ctime(time()),
                time_from_seconds(uptime()),
            }),
            "columns": 2,
            "align": "center",
        ]),
    ]));
    foreach (string line in border) {
        message("system", line + "\n", this_character());
    }
}