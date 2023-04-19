inherit STD_COMMAND;

void create () {
    set_syntax("heartbeats ([name filter]|[file filter])");
    set_help_text("The heartbeats command is used to view the list of objects with heartbeats.\n\nUsing a name or file filter will show the list of objects that match either filter.");
}

void command (string input, mapping flags) {
    object *obs;
    string *items = ({ });

    if (!input) {
        obs = heart_beats();
    } else {
        obs = filter(heart_beats(), (: $1 && ($1->query_key_name() == $(input) || regexp(file_name($1), $(input))) :));
    }

    foreach (object ob in obs) {
        items += ({ file_name(ob) });
    }

    border(([
        "title": "HEARTBEATS",
        "subtitle": input,
        "body": ({
            ([
                "items": items,
                "columns": 1,
            ])
        }),
    ]));
}