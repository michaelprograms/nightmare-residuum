#include <verb.h>

inherit STD_ABILITY;

void create () {
    ::create();
    set_type("attack");
    set_powers(([
        "brawl": 10,
    ]));
    set_targets(3);
    set_help_text("Whirl at your opponents.");
    set_cooldown(2);
}