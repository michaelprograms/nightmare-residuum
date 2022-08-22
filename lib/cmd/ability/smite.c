#include <verb.h>

inherit STD_ABILITY;

void create () {
    ::create();
    set_ability_requirements(([
        "paladin": ([
            "skills": ([
                "melee attack": 1,
                "psionic attack": 1,
            ]),
        ])
    ]));
    set_skill_powers(([
        "melee": 15,
        "psionic": 5,
    ]));
    set_help_text("Smite your opponent.");
}