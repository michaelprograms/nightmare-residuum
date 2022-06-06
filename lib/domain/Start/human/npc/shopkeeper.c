#include "human.h"

inherit STD_VENDOR;

void create () {
    ::create();

    set_name("Shopkeeper");
    set_short("shopkeeper");
    set_long("A shopkeeper.");
    set_level(1);
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
        ]));
    }
}