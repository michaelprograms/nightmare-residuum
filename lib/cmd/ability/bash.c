#include <verb.h>

inherit STD_ABILITY;

void create () {
    ::create();
    set_ability_type("attack");
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
    set_help_text("Bash your opponent with a blunt weapon.");
}