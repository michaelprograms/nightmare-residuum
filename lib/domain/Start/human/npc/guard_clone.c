#include "human.h"

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
    set_id(({ name, "clone", "guard", "dome guard", }));
    set_name(capName);
    set_short(capName + " the dome guard");
    set_long("A cloned human by the name of " + capName + ", working as a guard in the dome.");
    set_level(10);
    set_species("human");
    set_gender(element_of(({ "male", "female" })));

    set_ability_list(({ "bonk" }));
    set_ability_chance(50);

    set_inventory(([
        HUMAN_ARMOR + "hemp_pants.c": "wear pants",
        HUMAN_ARMOR + "hemp_shirt.c": "wear shirt",
        HUMAN_ARMOR + "wooden_shield": "wear shield",
    ]));
}