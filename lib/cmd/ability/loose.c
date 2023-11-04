#include <verb.h>

inherit STD_ABILITY;

void create () {
    ::create();
    set_type("attack");
    set_ability_requirements(([
        "ranger": ([
            "level": 1,
        ])
    ]));
    set_powers(([
        "ranged": 20,
    ]));
    set_help_text("Loose a ranged attack at your opponent.");
}

// @TODO custom attempt, fail and success messages for loose