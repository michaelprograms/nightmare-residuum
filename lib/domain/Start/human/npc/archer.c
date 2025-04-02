#include <human.h>

inherit STD_NPC;

void create () {
    ::create();
    set_gender(({ "male", "female" })[random(2)]);
    set_id(({ "archer", }));
    set_name("archer");
    set_short("a practicing archer");
    set_long("A cloned human practicing archery skills on a target.");
    set_level(5);
    set_species("human");

    set_ability_list(({ "loose", }));
    set_ability_chance(50);
}