#include <human.h>

inherit STD_NPC;

void create () {
    ::create();
    set_id(({ "adult", "regular adult", "human" }));
    set_name("regular adult");
    set_short("a regular adult");
    set_gender(element_of(({ "male", "female" })));
    set_long("A regular adult who ponders at your presence in " + possessive(this_object()) + " home.");
    set_level(8);
    set_species("human");
    set_ability_list(({ "bite" }));
    set_ability_chance(25);

    add_currency("copper", 10 + random(20));
}
