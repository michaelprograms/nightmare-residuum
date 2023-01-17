#include <verb.h>

inherit STD_ABILITY;

void create () {
    ::create();
    set_ability_type("heal");
    set_ability_requirements(([
        "psionist": ([
            "level": 1,
        ]),
    ]));
    set_skill_powers(([
        "medicine": 5,
    ]));
    set_help_text("Use psionic powers to mend a target's health.");
}