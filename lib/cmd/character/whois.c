inherit STD_COMMAND;

void create () {
    set_syntax("whois");
    set_help_text("The whois command is used to view the information about a character, even if they are disconnected.");
}

void command (string input, mapping flags) {
    object tc = this_character();
    object char;

    if (!input || input == "") {
        message("action", "Whois which character name?", tc);
        return;
    }

    char = D_CHARACTER->query_whois_character(input);
    if (!char) {
        message("action", "There is no character with that name on " + mud_name() + ".", tc);
        return;
    }

    border(([
        "title": "WHOIS",
        "subtitle": char->query_cap_name(),
        "header": ([
            "items": ({
                sprintf("%12s", "Account") + ": " + identify(char->query_account()), // @TODO
                sprintf("%12s", "Created") + ": " + strftime("%Y/%m/%u", char->query_created()),
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
                    capitalize(char->query_gender()) + " " + capitalize(char->query_species()),
                    capitalize(char->query_class()),
                    char->query_level(),
                }),
                "columns": 3,
                "align": "center",
            ]),
            ([
                "header": ({
                    "Experience",
                    "Victories",
                    "Victory Average",
                    "Defeats",
                }),
                "items": ({
                    format_integer(char->query_experience()),
                    char->query_victory(),
                    char->query_victory_average(),
                    sizeof(char->query_defeat()),
                }),
                "columns": 4,
                "align": "center",
            ]),
        }),
    ]));

    destruct(char);
}