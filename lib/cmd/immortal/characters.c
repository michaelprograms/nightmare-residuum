void command (string input, mapping flags) {
    mapping data = ([
        "title": "CHARACTERS",
        "subtitle": mud_name(),
    ]);
    string *items = ({ });

    foreach (object char in characters()) {
        items += ({
            sprintf("%-12s%-16s%s", time_from_seconds(query_idle(char->query_user()), 2), char->query_cap_name(), file_name(environment(char)))
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