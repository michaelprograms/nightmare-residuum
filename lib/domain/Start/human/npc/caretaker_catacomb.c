#include <human.h>

inherit STD_NPC;

void create () {
    ::create();
    set_id(({ "caretaker", "catacomb caretaker", "human" }));
    set_name("catacomb caretaker");
    set_short("a catacomb caretaker");
    set_gender(element_of(({ "male", "female" })));
    set_long("A caretaker of the catacombs.");
    set_level(8);
    set_species("human");
    set_class("mystic");
    set_ability_list(({ "pummel", }));
    set_ability_chance(25);

    add_currency("copper", 1 + random(10));
}