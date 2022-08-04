void command (string input, mapping flags) {
    object tc = this_character(), target = tc;
    string *border;
    object account;

    if (input && tc->query_immortal()) {
        if (find_character(input)) target = find_character(input);
        else if(present(input, environment(tc))) target = present(input, environment(tc));
    }

    border = format_border(([
        "title": "SETTINGS",
        "subtitle": target->query_cap_name(),
    ]));
    foreach (string line in border) {
        message("system", line + "\n", tc);
    }
}