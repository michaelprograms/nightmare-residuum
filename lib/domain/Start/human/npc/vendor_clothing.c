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
    set_id(({ name, "clone", "shopkeeper", "vendor", }));
    set_name(capName);
    set_short(capName + " the shopkeeper");
    set_long("A cloned human by the name of " + capName + ", working as the clothing vendor.");
    set_level(10);
    set_species("human");
    set_gender(element_of(({ "male", "female" })));

    if (clonep()) {
        query_vendor_inventory()->set_max_items(20);
        query_vendor_inventory()->set_reset(([
            HUMAN_ARMOR + "canvas_shoes.c": 1,
            HUMAN_ARMOR + "cloth_pants.c": 1,
            HUMAN_ARMOR + "cloth_shirt.c": 1,
            HUMAN_ARMOR + "leather_coat.c": 1,
            HUMAN_ARMOR + "leather_gloves.c": 1,
            HUMAN_ARMOR + "straw_hat.c": 1,
            HUMAN_ARMOR + "hemp_pants.c": 1,
            HUMAN_ARMOR + "hemp_shirt.c": 1,
        ]));
    }
    set_vendor_types(({ STD_ARMOR }));
    set_vendor_currency("copper");
}