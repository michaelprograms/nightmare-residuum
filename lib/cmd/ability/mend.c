#include <verb.h>

inherit STD_ABILITY;

void create () {
    ::create();
    set_type("heal");
    set_ability_requirements(([
        "psionist": ([
            "level": 1,
        ]),
    ]));
    // @TODO: rework this
    set_powers(([
        "medicine": 5,
    ]));
    set_help_text("Use psionic powers to mend a target's health.");
}