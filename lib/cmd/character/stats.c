inherit STD_COMMAND;

void create () {
    set_syntax("stats");
    set_help_text("The stats command is used to view the primary stats of your character.");
}

private int query_improve_percent (object target, string stat) {
    int exp = target->query_experience();
    int i, cost, pct;
    for (i = 0; i < 100; i ++) {
        if (exp <= 0) break;
        cost = D_EXPERIENCE->query_stat_cost(stat, target->query_stat_base(stat), target->query_class(), target->query_species());
        pct += max(({ min(({ exp * 100 / max(({ cost, 1 })), 100 })), 0 }));
        exp -= cost;
    }
    return pct;
}

void command (string input, mapping flags) {
    object tc = this_character(), target = tc;
    string *items = ({ });

    if (input && tc->query_immortal()) {
        if (find_character(input)) target = find_character(input);
        else if (present(input, environment(tc))) target = present(input, environment(tc));
    }

    foreach (string stat in ({ "strength", "perception", "endurance", "charisma", "intelligence", "agility", "luck", })) {
        int statBase = target->query_stat_base(stat);
        int statMax = D_CLASS->query_max_stat(target->query_class(), stat, target->query_level());
        int statBonus = target->query_stat_bonus(stat);
        int pct = query_improve_percent(target, stat);
        items += ({
            capitalize(stat),
            sprintf("%5s", ""+statBase),
            sprintf("%5s", ""+statMax),
            sprintf("%5s", ""+statBonus),
            sprintf("%12s", format_integer(D_EXPERIENCE->query_stat_cost(stat, statBase, target->query_class(), target->query_species()))),
            sprintf("%8s", format_integer(pct)+"%"),
        });
    }

    border(([
        "title": "STATS",
        "subtitle": target->query_cap_name(),
        "body": ([
            "header": ({ "Stat", "Base", "Max", "Bonus", "Cost", "Improve", }),
            "items": items,
            "columns": ({ 2, 1, 1, 1, 2, 1, }),
            "align": "left",
        ]),
        "footer": ([
            "items": ({
                "You have " + format_integer(target->query_experience()) + " experience points to "+format_syntax("improve [stat]")+" with.",
            }),
            "columns": 1,
            "align": "center",
        ])
    ]));
}