void command (string input, mapping flags) {
    string *border;

    border = format_border(([
        "title": "TIME",
        "body": ([
            "items": ({
                sprintf("%-18s", "Servertime:") + " " + ctime(time()),
                sprintf("%-18s", "Uptime:") + " " + time_from_seconds(uptime())
            }),
            "columns": 1,
        ]),
    ]));
    foreach (string line in border) {
        message("system", line + "\n", this_character());
    }
}