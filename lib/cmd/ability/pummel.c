#include <verb.h>

inherit STD_ABILITY;

void create () {
    ::create();
    set_type("attack");
    set_ability_requirements(([
        "mystic": ([
            "skills": ([
                "brawl attack": 1,
            ]),
        ])
    ]));
    set_skill_powers(([
        "brawl": 20,
    ]));
    set_weapons(([
        "brawl": ({ 1 }),
    ]));
    set_help_text("Pummel your opponent with your fist.");
}