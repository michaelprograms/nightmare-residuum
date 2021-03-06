void command (string input, mapping flags) {
    object tc = this_character();
    string *border;

    object char;

    if (!input || input == "") {
        message("action", "Whois which character name?\n", tc);
        return;
    }

    char = D_CHARACTER->query_whois_character(input);
    if (!char) {
        message("action", "There is no character with that name on " + mud_name() + ".\n", tc);
        return;
    }

    border = format_border(([
        "title": "SCORE",
        "subtitle": char->query_cap_name(),
        "header": ([
            "items": ({
                sprintf("%12s", "Account") + ": " + identify(char->query_account()), // @TODO
                sprintf("%12s", "Created") + ": " + strftime("%Y/%m/%u", char->query_created()),
            }),
            "columns": 2,
        ]),
        "body": ([
            "items": ({
                sprintf("%12s", "Species") + ": " + capitalize(char->query_gender()) + " " + capitalize(char->query_species()),
                sprintf("%12s", "Level") + ": " + char->query_level(),
                sprintf("%12s", "Victories") + ": " + char->query_victory(),
                sprintf("%12s", "Defeats") + ": " + char->query_defeat(),
            }),
            "columns": 2,
        ]),
    ]));
    foreach (string line in border) {
        message("system", line + "\n", tc);
    }

    destruct(char);
}