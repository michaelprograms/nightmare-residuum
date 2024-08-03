#include <human.h>

inherit STD_VENDOR;

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
    set_id(({ name, "clone", "attendant", "museum attendant", "entrance attendant", "museum entrance attendant" }));
    set_name(capName);
    set_short(capName + " the museum entrance attendant");
    set_long("A cloned human by the name of " + capName + ", working as an entrance attendant in the museum.");
    set_level(10);
    set_species("human");
    set_gender(element_of(({ "male", "female" })));

    set_ability_list(({ "bonk" }));
    set_ability_chance(50);

    if (clonep()) {
        query_vendor_inventory()->set_reset(([
            HUMAN_ITEM + "museum_ticket.c": 5,
        ]));
    }
    set_vendor_max_items(5);
    set_vendor_currency("copper");
}