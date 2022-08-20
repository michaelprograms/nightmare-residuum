#include <verb.h>

inherit STD_ABILITY;

void create () {
    ::create();
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
    set_help_text("Loose a ranged attack at your opponent.");
}

void handle_hit_msg (object source, object target) {
    message("action", "You loose an arrow at " + target->query_cap_name() + "!\n", source);
    message("action", source->query_cap_name() + " looses an arrow at you!\n", target);
    message("action", source->query_cap_name() + " looses an arrow at " + target->query_cap_name() + "!\n", environment(source), ({ source, target }));
}