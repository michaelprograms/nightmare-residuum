#include <verb.h>

inherit STD_ABILITY;

void create () {
    ::create();
    set_requirements(REQUIREMENT_BUSY | REQUIREMENT_DISABLE);
    set_ability_requirements(([
        "ranger": ([
            "skills": ([
                "ranged attack": 1,
            ]),
        ])
    ]));
    set_skill_powers(([
        "ranged": 20,
    ]));
}

void handle_hit_msg (object source, object target) {
    message("action", "You loose an arrow at " + target->query_cap_name() + "!\n", source);
    message("action", source->query_cap_name() + " looses an arrow at you!\n", target);
    message("action", source->query_cap_name() + " looses an arrow at " + target->query_cap_name() + "!\n", environment(source), ({ source, target }));
}

void help (object char) {
    message("help", "Loose a ranged attack at your opponent.\n", char);
}