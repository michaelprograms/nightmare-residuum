#include <human.h>

inherit STD_NPC;

void create () {
    ::create();
    set_id(({ "teen", "meditating teen", "human" }));
    set_name("meditating teen");
    set_short("a meditating teen");
    set_gender(element_of(({ "male", "female" })));
    set_long("A teenager meditating in silence, attempting to ignore all the noise in the world.");
    set_level(5);
    set_species("human");
    set_class("psionist");
    set_ability_list(({ "missile", "zap", }));
    set_ability_chance(25);

    add_currency("copper", 1 + random(10));
    set_posture("meditating");
}