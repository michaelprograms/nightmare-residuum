#include <verb.h>

inherit STD_ABILITY;

void create () {
    ::create();
    set_ability_type("attack");
    set_ability_requirements(([
        "mystic": ([
            "skills": ([
                "melee attack": 1,
            ]),
        ])
    ]));
    set_skill_powers(([
        "melee": 20,
    ]));
    set_weapons(([
        "melee": ({ 1 }),
    ]));
    set_help_text("Pummel your opponent with your fist.");
}