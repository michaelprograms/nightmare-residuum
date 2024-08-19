#include <human.h>

inherit STD_NPC;

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
    set_id(({ name, "clone", "mayor", "town mayor", }));
    set_name(capName);
    set_short(capName + " the Mayor");
    set_long("A cloned human by the name of " + capName + ", working as the mayor of the town.");
    set_level(10);
    set_species("human");

    set_ability_list(({ "bonk" }));
    set_ability_chance(50);
}

string *names = ({ });
void handle_receive_living_in_env (object living) {
    string name;
    ::handle_receive_living_in_env(living);

    if (!living->is_character()) return;

    name = living->query_cap_name();
    if (member_array(name, names) == -1) {
        handle_command("say Welcome to our town, " + name + "!");
        names += ({ name });
        call_out((: names -= ({ $1 }) :), 30, name);
    }
}