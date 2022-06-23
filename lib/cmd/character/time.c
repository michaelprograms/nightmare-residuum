void command (string input, mapping flags) {
    string *border, localtime, localdate;
    mapping a;

    a = D_ASTRONOMY->query_astronomy_from_room(environment(this_character()));
    localtime = D_ASTRONOMY->query_localtime(a);
    localdate = D_ASTRONOMY->query_localdate(a);

    border = format_border(([
        "title": "TIME",
        "header": ([
            "items": ({
                "Servertime",
                "Uptime",
                ctime(time()),
                time_from_seconds(uptime()),
            }),
            "columns": 2,
            "align": "center",
        ]),
        "body": ({
            ([
                "items": ({
                    sprintf("%-12s %s", "Localtime:", localtime),
                    sprintf("%-12s %s", "Localdate:", localdate),
                }),
                "columns": 2,
                "align": "center",
            ]),
        }),
    ]));
    foreach (string line in border) {
        message("system", line + "\n", this_character());
    }
}