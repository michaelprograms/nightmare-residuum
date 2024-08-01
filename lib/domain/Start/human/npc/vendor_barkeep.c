#include <human.h>

inherit STD_VENDOR;

void create () {
    ::create();
    set_id(({ "barkeep", }));
    set_name("barkeep");
    set_short("a barkeep");
    set_gender(element_of(({ "male", "female" })));
    set_long("A barkeep who awaits your order.");
    set_level(5);
    set_species("human");
    set_ability_list(({ "bonk", "bite", }));
    set_ability_chance(25);

    if (clonep()) {
        query_vendor_inventory()->set_reset(([

        ]));
    }
    set_vendor_types(({ STD_DRINK }));
    set_vendor_max_items(20);
    set_vendor_currency("copper");
}