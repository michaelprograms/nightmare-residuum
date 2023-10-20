#include <verb.h>

inherit STD_ABILITY;

void create () {
    ::create();
    set_type("attack");
    set_ability_requirements(([
        "anyone": ([
            "level": 1,
        ])
    ]));
    set_skill_powers(([
        "blunt": 30,
    ]));
    set_weapons(([
        "blunt": ({ 1, 2 }),
    ]));
    set_help_text("Bludgeon your opponent with a blunt weapon.");
}