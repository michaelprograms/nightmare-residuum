#include <human.h>

inherit STD_NPC;

void create () {
    ::create();
    set_id(({ "nurse", "human" }));
    set_name("nurse");
    set_short("a nurse");
    set_gender(element_of(({ "male", "female" })));
    set_long("A nurse treating patients in the clinic.");
    set_level(8);
    set_species("human");
    set_ability_list(({ "bite" }));
    set_ability_chance(25);

    add_currency("copper", 15 + random(10));
}
