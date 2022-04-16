void command (string input, mapping flags) {
    object tc = this_character(), target = tc;
    string *border, *items = ({ });

    if (input && tc->query_immortal()) {
        if (find_character(input)) target = find_character(input);
        else if(present(input, environment(tc))) target = present(input, environment(tc));
    }

    foreach (string type in ({ "head", "torso", "arm", "hand", "leg", "foot", "paw", "tail", })) {
        foreach (string l in sort_array(filter_array(target->query_limbs(), (: regexp($1, $(type)) :)), 1)) {
            mapping limb = target->query_limb(l);
            items += ({ sprintf("  %-16s %3s", l, (limb["damage"]*100/limb["maxdamage"])+"%") });
        }
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