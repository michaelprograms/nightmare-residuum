inherit STD_COMMAND;

void create () {
    set_syntax("hostiles");
    set_help_text("The hostiles command is used to view the list of things that are in combat with your character, even if they are in another location.");
}

void command (string input, mapping flags) {
    object tc = this_character(), target = tc;
    string *border, *items = ({ });

    if (input && tc->query_immortal()) {
        if (find_character(input)) target = find_character(input);
        else if (present(input, environment(tc))) target = present(input, environment(tc));
    }

    foreach (object ob in target->query_hostiles()) {
        items += ({ ob->query_cap_name() });
    }

    border = format_border(([
        "title": "HOSTILES",
        "subtitle": target->query_cap_name(),
        "body": ([
            "items": sizeof(items) ? items : ({ "No current hostiles" }),
            "columns": sizeof(items) ? 2 : 1,
            "align": sizeof(items) ? "left" : "center",
        ]),
    ]));
    foreach (string line in border) {
        message("system", line + "\n", tc);
    }
}