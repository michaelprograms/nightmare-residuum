inherit STD_COMMAND;

void create () {
    set_syntax("wanderers ([name filter]|[file filter])");
    set_help_text("The wanderers command is used to view the list of objects with wanderers.\n\nUsing a name or file filter will show the list of objects that match either filter.");
}

void command (string input, mapping flags) {
    object *obs;
    string *items = ({ });

    if (!input) {
        obs = filter(heart_beats(), (: $1 && $1->query_wander() :));
    } else {
        obs = filter(heart_beats(), (:
            $1 && $1->query_wander() &&
            ($1->query_key_name() == $(input) || regexp(file_name($1), $(input)))
        :));
    }

    foreach (object ob in obs) {
        items += ({
            file_name(ob),
            ob->query_wander(),
            ob->query_wanders(),
            ob->query_next_wander(),
            file_name(environment(ob)),
            "",
            "",
            "",
        });
    }

    border(([
        "title": "WANDERERS",
        "subtitle": input,
        "body": ({
            ([
                "header": ({ "Object", "Wander", "Wanders", "Next", }),
                "items": items,
                "columns": ({ 6, 1, 1, 1, }),
            ])
        }),
    ]));
}