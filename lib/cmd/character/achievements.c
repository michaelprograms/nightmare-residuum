inherit STD_COMMAND;

void create () {
    ::create();
    set_syntax("achievements (here)");
    set_help_text("The achievements command is used to view your achievements, or the achievements in your current environment.");
}

void command (string input, mapping flags) {
    mapping *body = ({ });

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
        mapping data = ([
            "title": "ACHIEVEMENTS",
            "body": ({ }),
        ]);
        string *done, *incomplete;
        done = this_character()->query_achievements_done();
        if (!sizeof(done)) {
            data["body"] += ({
                ([
                    "items": ({ "You have no achievements" }),
                    "align": "center",
                    "columns": 1,
                ])
            });
        } else {
            data["body"] = ({
                ([
                    "header": ({ "Complete" }),
                    "items": done
                ])
            });
        }
        incomplete = this_character()->query_achievements_incomplete();
        if (sizeof(incomplete)) {
            data["body"] += ({
                ([
                    "header": ({ "Incomplete" }),
                    "items": incomplete
                ])
            });
        }
        border(data);
    }
}