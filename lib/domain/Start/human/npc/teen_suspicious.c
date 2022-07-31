#include "human.h"

inherit STD_NPC;

void create () {
    ::create();
    set_id(({ "teen", "suspicious teen", "human" }));
    set_name("teen");
    set_short("a suspicious teen");
    set_gender(element_of(({ "male", "female" })));
    set_long("A natural born teenager who stares hard back at you, with a fierce attitude set upon " + possessive() + " face.");
    set_level(5);
    set_species("human");

    add_currency("copper", 1 + random(10));
}