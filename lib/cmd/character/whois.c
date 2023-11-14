inherit STD_COMMAND;

void create () {
    ::create();
    set_syntax("whois");
    set_help_text("The whois command is used to view the information about a character, even if they are disconnected.");
}

void command (string input, mapping flags) {
    object tc = this_character();
    object char, curChar;
    string *headerItems = ({ });

    if (!input || input == "") {
        message("action", "Whois which character name?", tc);
        return;
    }

    curChar = find_character(input); // need to do this before query_whois_character
    char = D_CHARACTER->query_whois_character(input);
    if (!char) {
        message("action", "There is no character with that name on " + mud_name() + ".", tc);
        return;
    }

    headerItems += ({
        sprintf("%12s", "Created") + ": " + strftime("%Y/%m/%u", char->query_created()),
    });
    if (curChar) {
        headerItems += ({
            sprintf("%12s", "Idle") + ": " + time_from_seconds(time() - curChar->query_last_action()),
        });
    } else {
        headerItems += ({
            sprintf("%12s", "Last on") + ": " + time_from_seconds(time() - char->query_last_action()),
        });
    }
    if (tc->query_immortal()) {
        headerItems += ({ sprintf("%12s", "Account") + ": " + char->query_account() });
    }

    border(([
        "title": "WHOIS",
        "subtitle": char->query_cap_name(),
        "header": ([
            "items": headerItems,
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