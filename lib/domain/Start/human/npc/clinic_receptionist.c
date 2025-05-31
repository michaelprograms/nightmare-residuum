#include <human.h>

inherit STD_NPC;

void create () {
    ::create();
    set_id(({ "receptionist", "human" }));
    set_name("receptionist");
    set_short("a receptionist");
    set_gender(element_of(({ "male", "female" })));
    set_long("A receptionist at the front desk of the clinic, checking patients in to be seen.");
    set_level(8);
    set_species("human");
    set_ability_list(({ "bite" }));
    set_ability_chance(35);

    add_currency("copper", 15 + random(10));
}
