#include <human.h>

inherit STD_VENDOR;

void create () {
    string name, capName;
    ::create();

    if (random(2)) {
        name = element_of(HUMAN_NAMES[0]);
        capName = capitalize(name);
        set_gender("male");
        set_id(({ name, "clone", "waiter", "vendor", }));
        set_short(capName + " the waiter");
    } else {
        name = element_of(HUMAN_NAMES[1]);
        capName = capitalize(name);
        set_gender("female");
        set_id(({ name, "clone", "waitress", "vendor", }));
        set_short(capName + " the waitress");
    }
    set_name(capName);
    set_long("A cloned human by the name of " + capName + ", working at a restaurant.");
    set_level(10);
    set_species("human");

    if (clonep()) {
        // query_vendor_inventory()->set_reset(([
        // @TODO
        // ]));
    }
    set_vendor_types(({ STD_FOOD }));
    set_vendor_max_items(20);
    set_vendor_currency("copper");
}