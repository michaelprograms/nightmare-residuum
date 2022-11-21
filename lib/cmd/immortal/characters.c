void command (string input, mapping flags) {
    mapping data = ([
        "title": "CHARACTERS",
        "subtitle": mud_name(),
    ]);
    string *items = ({ });
    object *characters = characters();

    foreach (object char in characters) {
        items += ({
            sprintf("%-20s%s", char->query_cap_name(), file_name(environment(char)))
        });
    }

    data["body"] = sizeof(items) ? ([
        "items": items,
        "columns": 1,
        "align": "left",
    ]) : ([
        "items": ({ "No player characters connected." }),
        "columns": 1,
        "align": "center",
    ]);

    border(data);
}