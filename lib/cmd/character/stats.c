void command (string input, mapping flags) {
    object tc = this_character(), target = tc;
    string *border, *items = ({ });

    if (input && tc->query_immortal()) {
        if (find_character(input)) target = find_character(input);
        else if (present(input, environment(tc))) target = present(input, environment(tc));
    }

    foreach (string stat in ({ "strength", "perception", "endurance", "charisma", "intelligence", "agility", "luck", })) {
        int statBase = target->query_stat_base(stat);
        int statBonus = target->query_stat_bonus(stat);
        items += ({
            stat,
            statBase,
            statBonus,
            format_integer(D_EXPERIENCE->query_stat_cost(stat, statBase, target->query_class(), target->query_species())) + " xp"
        });
    }

    border = format_border(([
        "title": "STATS",
        "subtitle": target->query_cap_name(),
        "body": ([
            "header": ({ "Stat", "Base", "Bonus", "Cost", }),
            "items": items,
            "columns": 4,
        ]),
        "footer": ([
            "items": ({
                "You have " + format_integer(target->query_experience()) + " experience points to train",
            }),
            "columns": 1,
            "align": "center",
        ])
    ]));
    foreach (string line in border) {
        message("system", line + "\n", tc);
    }
}