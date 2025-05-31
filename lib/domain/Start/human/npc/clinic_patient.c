#include <human.h>

inherit STD_NPC;

void create () {
    ::create();
    set_id(({ "patient", "human" }));
    set_name("patient");
    set_short("a patient");
    set_gender(element_of(({ "male", "female" })));
    set_long("A patient visiting the clinic to be treated.");
    set_level(8);
    set_species("human");
    set_ability_list(({ "bite" }));
    set_ability_chance(25);

    add_currency("copper", 5 + random(10));
}
