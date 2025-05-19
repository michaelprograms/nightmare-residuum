#include <human.h>

inherit STD_NPC;

void create () {
    ::create();
    set_id(({ "adult", "teacher", "human" }));
    set_name("teacher");
    set_short("a teacher");
    set_gender(element_of(({ "male", "female" })));
    set_long("A teacher who wonders why you are intruding upon " + possessive(this_object()) + " classroom.");
    set_level(8);
    set_species("human");
    set_ability_list(({ "bite" }));
    set_ability_chance(25);

    add_currency("copper", 10 + random(20));
}
