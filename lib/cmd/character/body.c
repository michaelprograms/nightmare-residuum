void command (string input, mapping flags) {
    object tc = this_character(), target = tc;
    string *border, *items = ({ }), *limbs, *severedLimbs;
    mapping footer;

    if (input && tc->query_immortal()) {
        if (find_character(input)) target = find_character(input);
        else if (present(input, environment(tc))) target = present(input, environment(tc));
    }

    limbs = target->query_limbs();
    severedLimbs = target->query_severed_limbs();
    foreach (string type in ({
        "head", "torso", "thorax", "abdomen",
        "arm", "hand", "leg", "foot",
        "paw", "wing", "tail",
    })) {
        foreach (string l in sort_array(filter_array(limbs, (: regexp($1, $(type)) :)), 1)) {
            mapping limb = target->query_limb(l);
            items += ({
                l,
                limb["damage"]+"/"+limb["maxdamage"] + " " + (limb["damage"]*100/limb["maxdamage"])+"%",
                limb["type"] ? limb["type"] : "",
                limb["attach"] && member_array(limb["attach"], severedLimbs) == -1 ? limb["attach"] : "",
            });
            limbs -= ({ l });
        }
    }
    foreach (string l in limbs) {
        mapping limb = target->query_limb(l);
        items += ({
            l,
            limb["damage"]+"/"+limb["maxdamage"] + " " + (limb["damage"]*100/limb["maxdamage"])+"%",
            limb["type"] ? limb["type"] : "",
            limb["attach"] ? limb["attach"] : "",
        });
    }

    if (sizeof(severedLimbs)) {
        footer = ([
            "items": ({
                "You are missing your " + conjunction(severedLimbs) + ".",
            }),
            "columns": 1,
            "align": "center",
        ]);
    }

    border = format_border(([
        "title": "BODY",
        "subtitle": target->query_cap_name(),
        "header": ([
            "items": ({
                "You have the body of a " + target->query_species(),
            }),
            "columns": 1,
            "align": "center",
        ]),
        "body": ([
            "header": ({ "Limb", "Damage", "Type", "Attached" }),
            "items": items,
            "columns": 4,
        ]),
        "footer": footer,
    ]));
    foreach (string line in border) {
        message("system", line + "\n", tc);
    }
}