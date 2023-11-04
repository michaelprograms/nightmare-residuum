#include <verb.h>

inherit STD_ABILITY;

void create () {
    ::create();
    set_type("attack");
    set_ability_requirements(([
        "NPC": ([
            "level": 1,
        ]),
    ]));
    set_powers(([
        "brawl": 10,
    ]));
    set_help_text("Scratch your opponent.");
}