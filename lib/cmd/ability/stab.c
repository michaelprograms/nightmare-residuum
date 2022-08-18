#include <verb.h>

inherit STD_ABILITY;

void create () {
    ::create();
    set_requirements(REQUIREMENT_BUSY | REQUIREMENT_DISABLE);
    set_ability_requirements(([
        "scoundrel": ([
            "skills": ([
                "knife attack": 1,
            ]),
        ])
    ]));
    set_skill_powers(([
        "knife": 20,
    ]));
    set_weapons(([
        "knife": ({ 1 }),
    ]));
}

void handle_hit_msg (object source, object target) {
    message("action", "You stab " + target->query_cap_name() + "!\n", source);
    message("action", source->query_cap_name() + " stabs you!\n", target);
    message("action", source->query_cap_name() + " stabs " + target->query_cap_name() + "!\n", environment(source), ({ source, target }));
}

void help (object char) {
    message("help", "Stab your opponent with a knife.\n", char);
}