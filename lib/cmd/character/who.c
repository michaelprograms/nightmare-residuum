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
    object char;
    int nImm = 0, nChar = 0;
    string *headerList = ({}), *bodyList = ({}), *footerList = ({});

    foreach (object user in users()) {
        string line;

        if (char = user->query_character()) {
            string characterName = char->query_cap_name();
            if (this_character()->query_immortal()) {
                characterName += " (" + user->query_account()->query_name() + ")";
            }
            if (this_user() != user) {
                characterName += " " + time_from_seconds(query_idle(user));
            }
            line = sprintf("%-50s", characterName) + sprintf("%-12s", capitalize(char->query_species())) + " " + sprintf("%3d", char->query_level());
            if (char->query_immortal()) {
                nImm ++;
                headerList += ({ line });
            } else {
                nChar ++;
                bodyList += ({ line });
            }
        } else {
            if (this_character()->query_immortal()) {
                nChar ++;
                bodyList += ({ user->query_account()->query_name()+" - "+identify(user) });
            }
        }
    }

    if (nImm > 0) {
        data["header"] = ([
            "items": headerList,
            "columns": 1,
        ]);
        footerList += ({ nImm + " immortal" + (nImm > 1 ? "s" : "") });
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