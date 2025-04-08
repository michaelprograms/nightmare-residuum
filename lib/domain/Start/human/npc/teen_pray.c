#include <human.h>

inherit STD_NPC;

void create () {
    ::create();
    set_id(({ "teen", "praying teen", "human" }));
    set_name("praying teen");
    set_short("a praying teen");
    set_gender(element_of(({ "male", "female" })));
    set_long("A teenager praying silently, trying to absolve the soul of any sins.");
    set_level(5);
    set_species("human");
    set_class("mystic");
    set_ability_list(({ "pummel", }));
    set_ability_chance(25);

    add_currency("copper", 1 + random(10));
    set_posture("sitting");
}