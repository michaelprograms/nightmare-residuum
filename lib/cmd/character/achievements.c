inherit STD_COMMAND;

void create () {
    ::create();
    set_syntax("achievements (here)");
    set_help_text("The achievements command is used to view your achievements, or the achievements in your current environment.");
}

void command (string input, mapping flags) {
    mapping a;
    string *body = ({ });

    if (input == "here") {
        a = D_ACHIEVEMENTS->query_achievements_from_room(environment(this_character()));
        foreach (mapping m in a || ([ ])) {
            body += ({
                ([
                    "items": ({
                        m["name"] + "\n" + m["description"],
                    }),
                    "columns": 1,
                    "align": "left",
                ]),
            });
        }

        border(([
            "title": "ACHIEVEMENTS",
            "body": body,
        ]));
    } else {
        // @TODO: Query your achievements
    }
}