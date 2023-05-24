inherit STD_COMMAND;

void create () {
    ::create();
    set_syntax("inventory");
    set_help_text("The inventory command is used to view the list of items being carried by your character.");
}

mapping process_section (object *items, string label, object target) {
    mapping section;

    if (sizeof(items)) {
        section = ([
            "header": ({ label, "Autoload", }),
            "items": ({ }),
            "columns": 2,
        ]);
        foreach (object *obs in unique_array(items, (: $1->query_short() :))) {
            string autoload;
            int a = sizeof(filter(obs->query_autoload(target), (: $1 :)));

            if (sizeof(obs) == 1) {
                autoload = (a ? "Yes" : "No");
            } else {
                autoload = (a == sizeof(obs) ? "All" : (a ? "Some" : "None"));
            }

            section["items"] += ({
                consolidate(sizeof(obs), obs[0]->query_short()),
                autoload,
            });
        }
    }

    return section;
}

mapping *process_inventory (object target) {
    object *weapon = ({ }), *armor = ({ }), *items = ({ });
    mapping *inventory = ({ });

    foreach (object item in target->query_item_contents()) {
        if (item->is_weapon()) {
            weapon += ({ item });
        } else if (item->is_armor()) {
            armor += ({ item });
        } else {
            items += ({ item });
        }
    }

    if (sizeof(weapon)) {
        inventory += ({ process_section(weapon, "Weapons", target) });
    }
    if (sizeof(armor)) {
        inventory += ({ process_section(armor, "Armor", target) });
    }
    if (sizeof(items)) {
        inventory += ({ process_section(items, "Items", target) });
    }

    if (!sizeof(inventory)) {
        inventory += ({
            ([
                "items": ({ "You are not carrying any items." }),
                "columns": 1,
                "align": "center",
            ])
        });
    }

    return inventory;
}

void command (string input, mapping flags) {
    object tc = this_character(), target = tc;
    mapping *items = ({ });
    mapping footer;
    string *coins = ({ });

    if (input && tc->query_immortal()) {
        if (find_character(input)) target = find_character(input);
        else if (present(input, environment(tc))) target = present(input, environment(tc));
    }

    items = process_inventory(target);

    foreach (string currency in target->query_currencies()) {
        int n;
        if (n = target->query_currency(currency)) {
            coins += ({ n + " " + currency });
        }
    }
    footer = ([
        "columns": 1,
        "align": "center",
    ]);
    if (sizeof(coins)) {
        footer["header"] = "Currencies";
        footer["items"] = ({ conjunction(coins), });
    } else {
        footer["header"] = "Currency";
        footer["items"] = ({ "You are not carrying any money.", });
    }

    border(([
        "title": "INVENTORY",
        "subtitle": target->query_cap_name(),
        "body": items,
        "footer": footer,
    ]));
}