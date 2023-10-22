#include "human.h"

// @TODO vendor
inherit STD_NPC;

void create () {
    ::create();
    set_id(({ "barkeep", }));
    set_name("barkeep");
    set_short("a barkeep");
    set_gender(element_of(({ "male", "female" })));
    set_long("A barkeep who awaits your order.");
    set_level(5);
    set_species("human");
    set_ability_list(({ "bonk", "bite", }));
    set_ability_chance(25);

    add_currency("copper", 5 + random(10));
}