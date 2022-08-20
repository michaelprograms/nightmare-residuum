#include <verb.h>

inherit STD_ABILITY;

void create () {
    ::create();
    set_requirements(REQUIREMENT_BUSY | REQUIREMENT_DISABLE);
    set_ability_requirements(([
        "templar": ([
            "skills": ([
                "melee attack": 1,
            ]),
        ])
    ]));
    set_skill_powers(([
        "melee": 20,
    ]));
    set_weapons(([
        "melee": ({ 1, }),
    ]));
    set_help_text("Pummel your opponent with your fist.");
}

void handle_hit_msg (object source, object target) {
    message("action", "You pummel " + target->query_cap_name() + "!\n", source);
    message("action", source->query_cap_name() + " pummels you!\n", target);
    message("action", source->query_cap_name() + " pummels " + target->query_cap_name() + "!\n", environment(source), ({ source, target }));
}