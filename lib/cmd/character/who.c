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
    string *border;
    int nImm = 0, nChar = 0;
    object *immortalList = ({ }), *charList = ({ });
    string *headerList = ({}), *bodyList = ({}), *footerList = ({ });

    foreach (object user in users()) {
        object char;
        if (char = user->query_character()) {
            if (char->query_immortal()) {
                nImm ++;
                immortalList += ({ char });
            } else {
                nChar ++;
                charList += ({ char });
            }
        }
    }

    if (nImm > 0) {
        immortalList = sort_array(immortalList, (: strcmp($1->query_name(), $2->query_name()) :));
        foreach (object char in immortalList) {
            string characterName = char->query_cap_name();
            if (this_character()->query_immortal()) {
                characterName += " (" + char->query_user()->query_account()->query_name() + ")";
            }
            if (this_user() != char->query_user()) {
                characterName += " " + time_from_seconds(query_idle(char->query_user()));
            }
            headerList += ({ sprintf("%-50s", characterName) + sprintf("%-12s", capitalize(char->query_species())) + " " + sprintf("%3d", char->query_level()) });
        }
        data["header"] = ([
            "items": headerList,
            "columns": 1,
        ]);
        footerList += ({ nImm + " immortal" + (nImm > 1 ? "s" : "") });
    }

    charList = sort_array(charList, (: strcmp($1->query_name(), $2->query_name()) :));
        foreach (object char in charList) {
            string characterName = char->query_cap_name();
            if (this_character()->query_immortal()) {
                characterName += " (" + char->query_user()->query_account()->query_name() + ")";
            }
            if (this_user() != char->query_user()) {
                characterName += " " + time_from_seconds(query_idle(char->query_user()));
            }
            bodyList += ({ sprintf("%-50s", characterName) + sprintf("%-12s", capitalize(char->query_species())) + " " + sprintf("%3d", char->query_level()) });
        }
    data["body"] = ([
        "items": sizeof(bodyList) ? bodyList : ({ "No player characters connected" }),
        "columns": 1,
        "align": sizeof(bodyList) ? "left" : "center",
    ]);
    footerList += ({ nChar + " character" + (nChar != 1 ? "s" : "") });
    data["footer"]["items"] = ({ implode(footerList, ", ") });

    border = format_border(data);
    foreach (string line in border) {
        message("system", line + "\n", this_character());
    }
}