void command (string input) {
    object tc = this_character(), target = tc;
    string *border;

    if (input && tc->query_immortal()) {
        if (find_character(input)) target = find_character(input);
        else if(present(input, environment(tc))) target = present(input, environment(tc));
    }

    border = format_border(([
        "title": "SCORE",
        "subtitle": target->query_cap_name(),
        "header": ([
            "items": ({
                sprintf("%12s", "Account") + ": " + target->query_user()->query_account()->query_name(),
                sprintf("%12s", "Created") + ": " + strftime("%Y/%m/%u", target->query_created()),
            }),
            "columns": 2,
        ]),
        "body": ([
            "items": ({
                sprintf("%12s", "Species") + ": " + capitalize(target->query_gender()) + " " + capitalize(target->query_species()),
                sprintf("%12s", "Level") + ": " + target->query_level(),
                sprintf("%12s", "Victories") + ": " + target->query_victory(),
                sprintf("%12s", "Defeats") + ": " + target->query_defeat(),
                sprintf("%12s", "Experience") + ": " + format_integer(target->query_experience()),
            }),
            "columns": 2,
        ]),
        "footer": ([
            "items": ({
                "Connected time: " + time_from_seconds(target->query_connection_time()),
            }),
            "columns": 1,
            "align": "center",
        ]),
    ]));
    foreach (string line in border) {
        message("system", line + "\n", tc);
    }

}