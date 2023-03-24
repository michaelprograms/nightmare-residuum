inherit STD_COMMAND;

void create () {
    set_syntax("who");
    set_help_text("The who command is used to view connected characters.");
}

void command (string input, mapping flags) {
    mapping data = ([
        "title": "WHO",
        "subtitle": mud_name(),
        "footer": ([
            "items": ({ }),
            "columns": 1,
            "align": "center",
        ]),
    ]);
    int nImm = 0, nChar = 0;
    object *immList = ({ }), *charList = ({ });
    string *bodyItems = ({ }), *footerItems = ({ });

    foreach (object user in users()) {
        object char;
        if (char = user->query_character()) {
            if (char->query_immortal()) {
                nImm ++;
                immList += ({ char });
            } else {
                nChar ++;
                charList += ({ char });
            }
        }
    }

    if (sizeof(immList) > 0) {
        immList = sort_array(immList, (: strcmp($1->query_name(), $2->query_name()) :));
        foreach (object imm in immList) {
            bodyItems += ({
                imm->query_level(),
                "%^BOLD%^" + imm->query_short() + "%^RESET%^",
                capitalize(imm->query_species()),
                capitalize(imm->query_class()),
            });
        }
        footerItems += ({ nImm + " immortal" + (nImm > 1 ? "s" : "") });
    }

    charList = sort_array(charList, (: strcmp($1->query_name(), $2->query_name()) :));
    foreach (object char in charList) {
        bodyItems += ({
            char->query_level(),
            char->query_short(),
            capitalize(char->query_species()),
            capitalize(char->query_class()),
        });
    }

    data["body"] = sizeof(bodyItems) ? ([
        "items": bodyItems,
        "columns": ({ 1, 4, 1, 1 }),
        "align": "left",
    ]) : ([
        "items": ({ "No player characters connected" }),
        "columns": 1,
        "align": "center",
    ]);
    footerItems += ({ nChar + " character" + (nChar != 1 ? "s" : "") });
    data["footer"]["items"] = ({ implode(footerItems, ", ") });

    border(data);
}