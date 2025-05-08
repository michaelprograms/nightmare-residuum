#include <human.h>

inherit STD_NPC;

void create () {
    ::create();
    set_id(({ "teen", "weightlifting teen", "human" }));
    set_name("weightlifting teen");
    set_short("a weightlifting teen");
    set_gender(element_of(({ "male", "female" })));
    set_long("A teenager who ignores your presence while " + possessive(this_object()) + " continues lifting weights.");
    set_level(5);
    set_species("human");
    set_ability_list(({ "bite" }));
    set_ability_chance(25);

    add_currency("copper", 5 + random(10));
}
