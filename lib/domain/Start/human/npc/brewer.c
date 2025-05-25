#include <human.h>

inherit STD_NPC;

void create () {
    ::create();
    set_id(({ "brewer", "human" }));
    set_name("brewer");
    set_short("a brewer");
    set_gender(element_of(({ "male", "female" })));
    set_long("A brewer making beer in the brewery.");
    set_level(8);
    set_species("human");
    set_class("warrior");
    set_ability_list(({ "bite", }));
    set_ability_chance(25);

    add_currency("copper", 5 + random(10));
}