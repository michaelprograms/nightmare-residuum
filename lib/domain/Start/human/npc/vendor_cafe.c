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
    set_id(({ name, "clone", "barista", "vendor", }));
    set_name(capName);
    set_short(capName + " the barista");
    set_long("A cloned human by the name of " + capName + ", working as cafe worker.");
    set_level(10);
    set_species("human");
    set_gender(element_of(({ "male", "female" })));

    if (clonep()) {
        query_vendor_inventory()->set_reset(([
            HUMAN_ITEM + "croissant.c": 4,
            HUMAN_ITEM + "tea.c": 4,
        ]));
    }
    set_vendor_types(({ STD_FOOD }));
    set_vendor_max_items(20);
    set_vendor_currency("copper");
}