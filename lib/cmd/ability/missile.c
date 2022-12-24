#include <verb.h>

inherit STD_ABILITY;

void create () {
    ::create();
    set_ability_type("attack");
    set_ability_requirements(([
        "psionist": ([
            "skills": ([
                "psionic attack": 1,
            ]),
        ])
    ]));
    set_skill_powers(([
        "psionic": 20,
    ]));
    set_help_text("Missile your opponent with your psychic powers.");
}