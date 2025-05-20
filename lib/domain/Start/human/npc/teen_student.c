#include <human.h>

inherit STD_NPC;

void create () {
    ::create();
    set_id(({ "teen", "student", "human" }));
    set_name("teen student");
    set_short("a teen student");
    set_gender(element_of(({ "male", "female" })));
    set_long("A teen student who wonders why you are disrupting class.");
    set_level(5);
    set_species("human");
    set_ability_list(({ "bite" }));
    set_ability_chance(25);

    add_currency("copper", 10 + random(20));
}
