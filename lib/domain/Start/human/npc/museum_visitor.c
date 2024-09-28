#include <human.h>

inherit STD_NPC;

void create () {
    string name, capName;
    ::create();

    if (random(2)) {
        name = element_of(HUMAN_NAMES[0]);
        set_gender("male");
    } else {
        name = element_of(HUMAN_NAMES[1]);
        set_gender("female");
    }
    capName = capitalize(name);
    set_id(({ name, "clone", "visitor", "museum visitor", }));
    set_name(capName);
    set_short(capName + " the museum visitor");
    set_long("A cloned human by the name of " + capName + ", visiting the museum.");
    set_level(10);
    set_species("human");

    set_ability_list(({ "bonk" }));
    set_ability_chance(50);
}