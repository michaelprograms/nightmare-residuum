#include <verb.h>

inherit STD_ABILITY;

void create () {
    ::create();
    set_ability_requirements(([
        "warrior": ([
            "skills": ([
                "blade attack": 10,
            ]),
        ])
    ]));
    set_skill_powers(([
        "blade": 15,
    ]));
    set_weapons(([
        "blade": ({ 1, 2 }),
    ]));
    set_help_text("Bash your opponent with a blunt weapon.");
}