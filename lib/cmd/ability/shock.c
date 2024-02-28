#include <verb.h>

inherit STD_ABILITY;

void create () {
    ::create();
    set_type("attack");
    set_ability_requirements(([
        "psionist": ([
            "level": 5,
        ])
    ]));
    set_powers(([
        "psionic": 10,
    ]));
    set_targets(3);
    set_help_text("Shock your opponent with your mind.");
    set_cooldown(2);
}