#include <human.h>

inherit STD_NPC;

void create () {
    ::create();
    set_id(({ "office worker", "worker", "human" }));
    set_name("office worker");
    set_short("an office worker");
    set_gender(element_of(({ "male", "female" })));
    set_long("An office worker who doesn't even notice you as " + subjective(this_object()) + " shuffles papers around like " + subjective(this_object()) + " is busy.");
    set_level(8);
    set_species("human");
    set_ability_list(({ "bite" }));
    set_ability_chance(25);

    add_currency("copper", 20 + random(10));
}
