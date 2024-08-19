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
    set_id(({ name, "clone", "mayor", "town mayor", }));
    set_name(capName);
    set_short(capName + " the Mayor");
    set_long("A cloned human by the name of " + capName + ", working as the mayor of the town.");
    set_level(10);
    set_species("human");
}