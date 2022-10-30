#include "human.h"

inherit STD_NPC;

void create () {
    ::create();
    set_id(({ "teen", "suspicious teen", "human" }));
    set_name("suspicious teen");
    set_short("a suspicious teen");
    set_gender(element_of(({ "male", "female" })));
    set_long("A natural born teenager who stares hard back at you, with a fierce attitude set upon " + possessive(this_object()) + " face.");
    set_level(5);
    set_species("human");
    set_ability_list(({ "stab" }));
    set_ability_chance(25);

    set_inventory(([
        HUMAN_WEAPON "bronze_dagger.c": "wield dagger",
    ]));
    add_currency("copper", 1 + random(10));
}

void handle_receive_living_in_env (object living) {
    ::handle_receive_living_in_env(living);
    if (!living->is_character()) return;
    if (random(3)) return;
    message("action", "You get the feeling that you are being watched.\n", environment(), this_object());
}