#include <verb.h>

inherit STD_ABILITY;

void create () {
    ::create();
    set_type("heal");
    set_ability_requirements(([
        "mystic": ([
            "level": 1,
        ]),
    ]));
    // @TODO: rework this
    set_powers(([
        "medicine": 5,
        "theurgy": 5,
    ]));
    set_targets(3);
    set_cooldown(2);
    set_help_text("Chant soothing words towards a target to restore hit points.");
}