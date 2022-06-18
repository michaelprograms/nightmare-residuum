void command (string input, mapping flags) {
    string *border;
    string localtime, localdate;

    localtime = D_ASTRONOMY->query_localtime(environment(this_character()));
    localdate = D_ASTRONOMY->query_localdate(environment(this_character()));

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