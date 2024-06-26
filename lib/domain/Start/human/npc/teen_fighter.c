#include <human.h>

inherit STD_NPC;

void create () {
    ::create();
    set_id(({ "teen", "fighting teen", "human" }));
    set_name("teen fighter");
    set_short("a fighting teen");
    set_gender(element_of(({ "male", "female" })));
    set_long("A teenager training to fight with wooden swords.");
    set_level(5);
    set_species("human");
    set_class("warrior");
    set_ability_list(({ "slash", }));
    set_ability_chance(25);

    set_inventory(([
        HUMAN_WEAPON + "wooden_sword.c": "wield sword",
    ]));
    add_currency("copper", 1 + random(10));
}

void handle_receive_living_in_env (object living) {
    ::handle_receive_living_in_env(living);
    if (!living->is_character()) return;
    if (random(2)) return;
    switch(random(4)) {
    case 0:
        handle_command("say Get a load of this wimp!");
        break;
    case 1:
        handle_command("say You think you can take me on?");
        break;
    case 2:
        handle_command("say " + capitalize(subjective(living)) + " thinks " + subjective(living) + " can fight?");
        break;
    case 3:
        handle_command("say Check out this limp noodle.");
        break;
    }
}