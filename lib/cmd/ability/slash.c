#include <verb.h>

inherit STD_ABILITY;

void create () {
    ::create();
    set_type("attack");
    set_ability_requirements(([
        "warrior": ([
            "level": 5,
        ])
    ]));
    set_skill_powers(([
        "blade": 15,
    ]));
    set_weapons(([
        "blade": ({ 1, 2 }),
    ]));
    set_help_text("Slash your opponent with a bladed weapon.");
}