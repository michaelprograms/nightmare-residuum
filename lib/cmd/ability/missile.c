#include <verb.h>

inherit STD_ABILITY;

void create () {
    ::create();
    set_requirements(REQUIREMENT_BUSY | REQUIREMENT_DISABLE);
    set_ability_requirements(([
        "mentalist": ([
            "skills": ([
                "psionic attack": 5,
            ]),
        ])
    ]));
    set_skill_powers(([
        "psionic": 20,
    ]));
}

void handle_hit_msg (object source, object target) {
    message("action", "You missile " + target->query_cap_name() + "!\n", source);
    message("action", source->query_cap_name() + " missiles you!\n", target);
    message("action", source->query_cap_name() + " missiles " + target->query_cap_name() + "!\n", environment(source), ({ source, target }));
}