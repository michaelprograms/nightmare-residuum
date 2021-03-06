void command (string input, mapping flags) {
    string *border, localtime, localdate;
    mapping a;

    a = D_ASTRONOMY->query_astronomy_from_room(environment(this_character()));
    localtime = D_ASTRONOMY->query_localtime(a);
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
            "header": ({ "Servertime", "Uptime" }),
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