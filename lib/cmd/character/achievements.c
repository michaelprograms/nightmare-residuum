inherit STD_COMMAND;

void create () {
    ::create();
    set_syntax("achievements (here)");
    set_help_text("The achievements command is used to view your achievements, or the achievements in your current environment.");
}

void command (string input, mapping flags) {
    string *body = ({ });

    if (input == "here") {
        mapping a = D_ACHIEVEMENTS->query_achievements_from_room(environment(this_character()));
        if (!sizeof(a)) {
            body += ({
                ([
                    "items": ({
                        "No achievements found",
                    }),
                    "columns": 1,
                    "align": "center",
                ]),
            });
        } else {
            foreach (mapping m in a) {
                body += ({
                    ([
                        "items": ({
                            "%^BOLD%^UNDERLINE%^" + m["name"] + "%^RESET%^ \n" + m["description"],
                        }),
                        "columns": 1,
                        "align": "left",
                    ]),
                });
            }
        }

        border(([
            "title": "ACHIEVEMENTS",
            "body": body,
        ]));
    } else {
        string *a = this_character()->query_achievements();
        if (!sizeof(a)) {
            a = ({ "You have no achievements" });
        }
        border(([
            "title": "ACHIEVEMENTS",
            "body": ([
                "items": a
            ]),
        ]));
    }
}