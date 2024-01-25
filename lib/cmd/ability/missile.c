#include <verb.h>

inherit STD_ABILITY;

void create () {
    ::create();
    set_type("attack");
    set_ability_requirements(([
        "psionist": ([
            "level": 3,
        ])
    ]));
    set_powers(([
        "psionic": 25,
    ]));
    set_help_text("Missile your opponent with psychic powers.");
}