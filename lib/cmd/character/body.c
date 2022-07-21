void command (string input, mapping flags) {
    object tc = this_character(), target = tc;
    string *border, *items = ({ }), *limbs;

    if (input && tc->query_immortal()) {
        if (find_character(input)) target = find_character(input);
        else if(present(input, environment(tc))) target = present(input, environment(tc));
    }

    limbs = target->query_limbs();
    foreach (string type in ({
        "head", "torso", "thorax", "abdomen",
        "arm", "hand", "leg", "foot",
        "paw", "wing", "tail",
    })) {
        foreach (string l in sort_array(filter_array(limbs, (: regexp($1, $(type)) :)), 1)) {
            mapping limb = target->query_limb(l);
            items += ({ sprintf("  %-24s %3s", l, (limb["damage"]*100/limb["maxdamage"])+"%") });
            limbs -= ({ l });
        }
    }
    foreach (string l in limbs) {
        mapping limb = target->query_limb(l);
        items += ({ sprintf("  %-24s %3s", l, (limb["damage"]*100/limb["maxdamage"])+"%") });
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
            "items": items,
            "columns": 2,
        ]),
    ]));
    foreach (string line in border) {
        message("system", line + "\n", tc);
    }
}