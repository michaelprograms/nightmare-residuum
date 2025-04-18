#include <human.h>

inherit STD_NPC;

void create () {
    ::create();
    set_id(({ "teen", "preppy teen", "human" }));
    set_name("preppy teen");
    set_short("a preppy teen");
    set_gender(element_of(({ "male", "female" })));
    set_long("A teenager who stares back at you, with an air of disgust upon " + possessive(this_object()) + " face.");
    set_level(5);
    set_species("human");
    set_ability_list(({ "bite" }));
    set_ability_chance(25);

    add_currency("copper", 5 + random(10));
}
