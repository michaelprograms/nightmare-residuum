void command (string input, mapping flags) {
    mapping data = ([
        "title": "CHARACTERS",
        "subtitle": mud_name(),
    ]);
    object tc = this_character();
    string *border, *bodyItems = ({ });
    object *characters = characters();

    foreach (object char in characters) {
        bodyItems += ({
            sprintf("%-20s%s", char->query_cap_name(), file_name(environment(char)))
        });
    }

    data["body"] = sizeof(bodyItems) ? ([
        "items": bodyItems,
        "columns": 1,
        "align": "left",
    ]) : ([
        "items": ({ "No player characters connected" }),
        "columns": 1,
        "align": "center",
    ]);

    border = format_border(data);
    foreach (string line in border) {
        message("system", line + "\n", tc);
    }
}