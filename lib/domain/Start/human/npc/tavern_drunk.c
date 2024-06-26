#include "human.h"

inherit STD_NPC;

void create () {
    ::create();
    set_id(({ "drunk", "tavern drunk", }));
    set_name("drunk");
    set_short("a drunk outside the tavern");
    set_gender(element_of(({ "male", "female" })));
    set_long("A drunk who stares through you with crossed eyes and a blank angry face, while hiccuping.");
    set_level(5);
    set_species("human");
    set_ability_list(({ "stab" }));
    set_ability_chance(25);

    set_inventory(([
        HUMAN_WEAPON + "broken_bottle.c": "wield bottle",
    ]));
    add_currency("copper", 1 + random(10));
}