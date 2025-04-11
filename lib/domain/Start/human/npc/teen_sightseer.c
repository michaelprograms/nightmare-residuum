#include <human.h>

inherit STD_NPC;

void create () {
    ::create();
    set_id(({ "teen", "sightseer teen", "human" }));
    set_name("sightseer teen");
    set_short("a sightseer teen");
    set_gender(element_of(({ "male", "female" })));
    set_long("A teenager sightseer taking in the views.");
    set_level(5);
    set_species("human");
    set_class("mystic");
    set_ability_list(({ "pummel", }));
    set_ability_chance(25);

    add_currency("copper", 1 + random(10));
}