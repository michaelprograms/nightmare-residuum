inherit STD_COMMAND;

void create () {
    set_syntax("defeats");
    set_help_text("The defeats command is used to view the list of defeats your character has recorded.");
}

void command (string input, mapping flags) {
    object tc = this_character(), target = tc;
    string *items = ({ });

    if (input && tc->query_immortal()) {
        if (find_character(input)) target = find_character(input);
        else if (present(input, environment(tc))) target = present(input, environment(tc));
    }

    foreach (mixed *defeat in target->query_defeat()) {
        items += ({ (defeat[0] ? defeat[0] : "Unknown"), ctime(defeat[1]) });
    }

    border(([
        "title": "DEFEATS",
        "subtitle": target->query_cap_name(),
        "body": ([
            "items": sizeof(items) ? items : ({ "No defeats." }),
            "columns": sizeof(items) ? 2 : 1,
            "align": sizeof(items) ? "left" : "center",
        ]),
    ]));
}