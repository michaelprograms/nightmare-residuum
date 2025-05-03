#include <human.h>

inherit STD_NPC;

void create () {
    ::create();
    set_id(({ "adult", "preppy adult", "human" }));
    set_name("preppy adult");
    set_short("a preppy adult");
    set_gender(element_of(({ "male", "female" })));
    set_long("An adult who scoffs at your presence.");
    set_level(5);
    set_species("human");
    set_ability_list(({ "bite" }));
    set_ability_chance(25);

    add_currency("copper", 10 + random(20));
}
