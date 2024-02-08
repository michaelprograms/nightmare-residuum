#include "origin.h"

inherit STD_VENDOR;

void create () {
    ::create();
    set_name("R-1001001");
    set_id(({ "synthetic creature", "synthetic", "creature", "vendor", }));
    set_short("a synthetic creature");
    set_long("A creature seemingly constucted of synthetic components. It consists of a large elongated torso structure supported by four jointed legs, all encased in a scale-like exoskeloton. There are two articulated arms at each end of the structure, almost looking like a type of crustacean. The creature does seem to have a discernable head or face.");
    set_species("custodian");

    if (clonep()) {
        query_vendor_inventory()->set_max_items(20);
        query_vendor_inventory()->set_reset(([
            ORIGIN_ITEM + "syringe_healing_nanites.c": 10,
            ORIGIN_ITEM + "syringe_damaging_nanites.c": 5,
        ]));
    }
    set_vendor_types(({ STD_INJECTABLE }));
    set_vendor_currency("copper");
}