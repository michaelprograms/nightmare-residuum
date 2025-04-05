#include <human.h>

inherit STD_NPC;

void create () {
    ::create();
    set_id(({ "adult", "meditating adult", "human" }));
    set_name("meditating adult");
    set_short("a meditating adult");
    set_gender(element_of(({ "male", "female" })));
    set_long("An adult meditating in silence, ignoring all the noise of the world.");
    set_level(8);
    set_species("human");
    set_class("psionist");
    set_ability_list(({ "missile", "zap", }));
    set_ability_chance(25);

    add_currency("copper", 2 + random(15));
    set_posture("meditating");
}