#include <human.h>

inherit STD_NPC;

void create () {
    ::create();
    set_id(({ "adult", "worshiping adult", "human" }));
    set_name("worshiping adult");
    set_short("a worshiping adult");
    set_gender(element_of(({ "male", "female" })));
    set_long("A worshiping adult who pays you no attention.");
    set_level(8);
    set_species("human");
    set_ability_list(({ "bite" }));
    set_ability_chance(25);

    add_currency("copper", 10 + random(20));
}
