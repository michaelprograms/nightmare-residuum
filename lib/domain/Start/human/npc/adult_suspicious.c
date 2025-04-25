#include <human.h>

inherit STD_NPC;

void create () {
    ::create();
    set_id(({ "adult", "suspicious adult", "human" }));
    set_name("suspicious adult");
    set_short("a suspicious adult");
    set_gender(element_of(({ "male", "female" })));
    set_long("An adult who glares at you.");
    set_level(8);
    set_species("human");
    set_class("scoundrel");
    set_ability_list(({ "stab" }));
    set_ability_chance(25);

    set_inventory(([
        HUMAN_WEAPON + "bronze_dagger.c": "wield dagger",
    ]));
    add_currency("copper", 5 + random(10));
}

void handle_receive_living_in_env (object living) {
    ::handle_receive_living_in_env(living);
    if (!characterp(living)) return;
    if (random(3)) return;
    handle_command("say What are you doing in here?!");
}