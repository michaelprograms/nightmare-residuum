inherit STD_COMMAND;

void create () {
    ::create();
    set_syntax("history");
    set_help_text("The history command is used to view the list of the last 40 commands you have entered for your character.");
}

void command (string input, mapping flags) {
    object tc = this_character(), target = tc;
    string *items = ({ });
    string *history;

    if (input && tc->query_immortal()) {
        target = determine_immortal_target(tc, input);
    }

    history = target->query_user()->query_history();
    for (int i = 0; i < sizeof(history); i ++) {
        items += ({ sprintf("%3d", i + 1) + " " + history[i] });
    }

    border(([
        "title": "HISTORY",
        "subtitle": target->query_cap_name(),
        "body": ([
            "items": sizeof(items) ? items : ({ "No history." }),
            "columns": 1,
            "align": sizeof(items) ? "left" : "center",
        ]),
    ]));
}