void command (string input, mapping flags) {
    object tc = this_character();
    string *msgs;
    string *border, *body = ({ }), subtitle;

    if (input == "tells" || input == "tell") {
        subtitle = "tells";
        if (!sizeof(msgs = tc->query_tell_history())) {
            body += ({ "No one has told you anything." });
        } else {
            body += msgs;
        }
    } else if (input == "says" || input == "say") {
        subtitle = "says";

        if (!sizeof(msgs = tc->query_say_history())) {
            body += ({ "No one has said you anything." });
        } else {
            body += msgs;
        }
    } else {
        message("action", "Syntax: <last says|tells>\n", tc);
        return;
    }

    write("body is: "+identify(body)+"\n");
    border = format_border(([
        "title": "LAST",
        "subtitle": subtitle,
        "body": ([
            "items": body,
            "align": "left",
            "columns": 1,
        ]),
    ]));
    foreach (string line in border) {
        message("system", line + "\n", tc);
    }
}