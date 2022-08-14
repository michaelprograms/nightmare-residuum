#include <verb.h>

inherit STD_ABILITY;

void create () {
    ::create();
    set_requirements(REQUIREMENT_BUSY | REQUIREMENT_DISABLE);
    set_ability_requirements(([
        "scoundrel": ([
            "skills": ([
                "melee attack": 2,
                "knife attack": 2,
            ]),
        ])
    ]));
    set_skill_powers(([
        "melee attack": 10,
        "knife attack": 10,
    ]));
}

void handle_hit_msg (object source, object target) {
    message("action", "You stab " + target->query_cap_name() + "!\n", source);
    message("action", source->query_cap_name() + " stabs you!\n", target);
    message("action", source->query_cap_name() + " stabs " + target->query_cap_name() + "!\n", environment(source), ({ source, target }));
}