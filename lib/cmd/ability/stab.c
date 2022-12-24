#include <verb.h>

inherit STD_ABILITY;

void create () {
    ::create();
    set_ability_type("attack");
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
    set_difficulty_factor(110);
    set_help_text("Stab your opponent with a knife.");
}