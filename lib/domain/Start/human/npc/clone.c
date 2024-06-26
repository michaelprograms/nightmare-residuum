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
    set_id(({ name, "clone" }));
    set_name(capName);
    set_short(capName + " the clone");
    set_long("A cloned human by the name of " + capName + ".");
    set_level(10);
    set_species("human");

    set_inventory(([
        HUMAN_ARMOR + "hemp_pants.c": "wear pants",
        HUMAN_ARMOR + "hemp_shirt.c": "wear shirt",
    ]));
    add_currency("copper", 1 + random(10));
}