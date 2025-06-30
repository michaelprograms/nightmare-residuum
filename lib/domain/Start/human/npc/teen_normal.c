#include <human.h>

inherit STD_NPC;

void create () {
    ::create();
    set_id(({ "teen", "regular teen", "human" }));
    set_name("regular teen");
    set_short("a regular teen");
    set_gender(element_of(({ "male", "female" })));
    set_long("A regular teen who runs in shenanigans around paying you no attention.");
    set_level(5);
    set_species("human");
    set_ability_list(({ "bite" }));
    set_ability_chance(25);

    add_currency("copper", 10 + random(20));
}
