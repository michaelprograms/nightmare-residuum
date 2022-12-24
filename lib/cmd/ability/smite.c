#include <verb.h>

inherit STD_ABILITY;

void create () {
    ::create();
    set_ability_type("attack");
    set_ability_requirements(([
        "paladin": ([
            "skills": ([
                "brawl attack": 1,
                "psionic attack": 1,
            ]),
        ])
    ]));
    set_skill_powers(([
        "brawl": 15,
        "psionic": 5,
    ]));
    set_help_text("Smite your opponent.");
}