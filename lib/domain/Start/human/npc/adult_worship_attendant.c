#include <human.h>

inherit STD_NPC;

void create () {
    ::create();
    set_id(({ "adult", "worship attendant adult", "human" }));
    set_name("worship attendant");
    set_short("a worship attendant");
    set_gender(element_of(({ "male", "female" })));
    set_long("An adult worship attendant who pays you no attention while doing preparations.");
    set_level(8);
    set_species("human");
    set_ability_list(({ "bite" }));
    set_ability_chance(25);

    add_currency("copper", 10 + random(20));
}
