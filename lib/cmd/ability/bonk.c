#include <verb.h>

inherit STD_ABILITY;

void create () {
    ::create();
    set_ability_type("attack");
    set_skill_powers(([
        "brawl": 10,
    ]));
    set_help_text("Bonk your opponent with your forehead.");
}