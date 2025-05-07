#include <human.h>

inherit STD_NPC;

void create () {
    ::create();
    set_id(({ "adult", "adult", "human" }));
    set_name("adult");
    set_short("an adult lifting weights");
    set_gender(element_of(({ "male", "female" })));
    set_long("An adult who ignores your presence while " + possessive(this_object()) + " continues lifting weights.");
    set_level(8);
    set_species("human");
    set_ability_list(({ "bite" }));
    set_ability_chance(25);

    add_currency("copper", 10 + random(20));
}
