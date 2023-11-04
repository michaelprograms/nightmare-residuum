#include <verb.h>

inherit STD_ABILITY;

void create () {
    ::create();
    set_type("attack");
    set_powers(([
        "brawl": 10,
    ]));
    set_help_text("Bonk your opponent with your forehead.");
}