#include <verb.h>

inherit STD_ABILITY;

void create () {
    ::create();
    set_skill_powers(([
        "melee": 10,
    ]));
    set_help_text("Bonk your opponent with your forehead.");
}