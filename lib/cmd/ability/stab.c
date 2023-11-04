#include <verb.h>

inherit STD_ABILITY;

void create () {
    ::create();
    set_type("attack");
    set_ability_requirements(([
        "scoundrel": ([
            "level": 1,
        ])
    ]));
    set_powers(([
        "blade": 20,
    ]));
    set_weapons(([
        "blade": ({ 1 }),
    ]));
    set_difficulty_factor(110);
    set_help_text("Stab your opponent with a blade.");
}