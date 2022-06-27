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
    object *immList = ({ }), *charList = ({ });
    string *headerItems = ({ }), *bodyItems = ({ }), *footerItems = ({ });

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
        headerItems += ({
            conjunction(map(immList, (: "%^MAGENTA%^BOLD%^"+$1->query_cap_name()+"%^RESET%^" :)))
        });
        data["header"] = ([
            "header": ({ "Immortals" }),
            "items": headerItems,
            "columns": 1,
            "align": "center",
        ]);
        footerItems += ({ nImm + " immortal" + (nImm > 1 ? "s" : "") });
    }

    charList = sort_array(charList, (: strcmp($1->query_name(), $2->query_name()) :));
        foreach (object char in charList) {
            bodyItems += ({
                char->query_level(),
                char->query_cap_name(),
                capitalize(char->query_species()),
                capitalize(char->query_class()),
            });
        }
    data["body"] = sizeof(bodyItems) ? ([
        "items": bodyItems,
        "columns": 4,
        "align": "left",
    ]) : ([
        "items": ({ "No player characters connected" }),
        "columns": 1,
        "align": "center",
    ]);
    footerItems += ({ nChar + " character" + (nChar != 1 ? "s" : "") });
    data["footer"]["items"] = ({ implode(footerItems, ", ") });

    border = format_border(data);
    foreach (string line in border) {
        message("system", line + "\n", this_character());
    }
}