void command (string input, mapping flags) {
    object tc = this_character(), target = tc;
    string *border;
    object account;
    string *list = ({ });

    if (input && tc->query_immortal()) {
        if (find_character(input)) target = find_character(input);
        else if(present(input, environment(tc))) target = present(input, environment(tc));
    }

    account = target->query_user()->query_account();

    foreach (string key,mixed value in account->query_settings()) {
        list += ({ sprintf("%16s : %s", key, ""+value) });
    }

    border = format_border(([
        "title": "SETTINGS",
        "subtitle": target->query_cap_name(),
        "body": ([
            "items": list,
            "columns": 1,
        ]),
    ]));
    foreach (string line in border) {
        message("system", line + "\n", tc);
    }
}