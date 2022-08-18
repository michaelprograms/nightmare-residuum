#include <verb.h>

inherit STD_ABILITY;

void create () {
    ::create();
    set_requirements(REQUIREMENT_BUSY | REQUIREMENT_DISABLE);
    set_ability_requirements(([
        "warrior": ([
            "skills": ([
                "blunt attack": 1,
            ]),
        ])
    ]));
    set_skill_powers(([
        "blunt": 20,
    ]));
    set_weapons(([
        "blunt": ({ 1, 2 }),
    ]));
}

void handle_hit_msg (object source, object target) {
    message("action", "You bash " + target->query_cap_name() + "!\n", source);
    message("action", source->query_cap_name() + " bashes you!\n", target);
    message("action", source->query_cap_name() + " bashes " + target->query_cap_name() + "!\n", environment(source), ({ source, target }));
}


void help (object char) {
    message("help", "Bash your opponent with a blunt weapon.\n", char);
}