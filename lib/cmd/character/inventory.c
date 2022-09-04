void command (string input, mapping flags) {
    object tc = this_character(), target = tc;
    object *inv;
    string *coins = ({ }), *items = ({ });
    string *border;
    mapping footer;

    if (input && tc->query_immortal()) {
        if (find_character(input)) target = find_character(input);
        else if(present(input, environment(tc))) target = present(input, environment(tc));
    }

    inv = unique_array(all_inventory(target), (: $1->query_short("%^RESET%^") :));
    if (sizeof(inv)) {
        string *shorts = map(sort_array(map(inv, (: capitalize(consolidate(sizeof($1), $1[0]->query_short("%^RESET%^"))) :)), 1), (: $1 :));
        foreach (string short in shorts) {
            items += ({ short });
        }
    } else {
        items = ({ "You have no items." });
    }

    foreach (string currency in target->query_currencies()) {
        int n;
        if (n = target->query_currency(currency)) {
            coins += ({ n + " " + currency });
        }
    }
    if (sizeof(coins)) {
        footer = ([
            "header": "Currenc" + (sizeof(coins) > 1 ? "ies" : "y"),
            "items": ({
                 conjunction(coins),
            }),
            "columns": 1,
            "align": "center",
        ]);
    }

    border = format_border(([
        "title": "INVENTORY",
        "subtitle": target->query_cap_name(),
        "body": ([
            "header": "Carried Items",
            "items": items,
            "columns": 1,
            "align": "left",
        ]),
        "footer": footer,
    ]));
    foreach (string line in border) {
        message("system", line + "\n", tc);
    }

}