void command (string input) {
    object tc = this_character(), target = tc;
    string *border, subtitle, *items = ({ });

    if (input && tc->query_immortal()) {
        if (find_character(input)) target = find_character(input);
        else if(present(input, environment(tc))) target = present(input, environment(tc));
    }

    foreach (string stat in ({ "strength", "perception", "endurance", "charisma", "intelligence", "agility", "luck", })) {
        items += ({ sprintf("%-15s %3d %12s xp", stat, target->query_stat(stat), format_integer(D_EXPERIENCE->query_stat_cost(stat, target->query_stat(stat)))) });
    }

    border = format_border(([
        "title": "STATS",
        "subtitle": target->query_cap_name(),
        "body": ([
            "items": items,
            "columns": 1,
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