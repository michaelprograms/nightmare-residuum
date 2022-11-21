inherit STD_COMMAND;

void create () {
    set_syntax("score");
    set_help_text("The score command is used to view the attributes about your character.");
}

void command (string input, mapping flags) {
    object tc = this_character(), target = tc;

    if (input && tc->query_immortal()) {
        if (find_character(input)) target = find_character(input);
        else if (present(input, environment(tc))) target = present(input, environment(tc));
    }

    border(([
        "title": "SCORE",
        "subtitle": target->query_cap_name(),
        "header": ([
            "items": ({
                sprintf("%12s", "Account") + ": " + target->query_user()->query_account()->query_name(),
                sprintf("%12s", "Created") + ": " + strftime("%Y/%m/%u", target->query_created()),
            }),
            "columns": 2,
        ]),
        "body": ({
            ([
                "header": ({
                    "Species",
                    "Class",
                    "Level",
                }),
                "items": ({
                    capitalize(target->query_gender()) + " " + capitalize(target->query_species()),
                    capitalize(target->query_class()),
                    target->query_level(),
                }),
                "columns": 3,
                "align": "center",
            ]),
            ([
                "header": ({
                    "Experience",
                    "Victories",
                    "Defeats",
                }),
                "items": ({
                    format_integer(target->query_experience()),
                    target->query_victory(),
                    target->query_defeat(),
                }),
                "columns": 3,
                "align": "center",
            ]),
            ([
                "header": ({
                    "STR",
                    "PER",
                    "END",
                    "CHA",
                    "INT",
                    "AGI",
                    "LUCK",
                }),
                "items": ({
                    target->query_stat("strength"),
                    target->query_stat("perception"),
                    target->query_stat("endurance"),
                    target->query_stat("charisma"),
                    target->query_stat("intelligence"),
                    target->query_stat("agility"),
                    target->query_stat("luck"),
                }),
                "columns": 7,
                "align": "center",
            ]),
            ([
                "header": ({
                    "HP",
                    "SP",
                    "MP",
                }),
                "items": ({
                    target->query_hp() + " / " + target->query_max_hp(),
                    target->query_sp() + " / " + target->query_max_sp(),
                    target->query_mp() + " / " + target->query_max_mp(),
                }),
                "columns": 3,
                "align": "center",
            ]),
        }),
        "footer": ([
            "items": ({
                "Connected time: " + time_from_seconds(target->query_connection_time()),
            }),
            "columns": 1,
            "align": "center",
        ]),
    ]));
}