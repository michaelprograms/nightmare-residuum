#include <verb.h>

inherit STD_VERB;

void create () {
    verb::create();
    add_rules( ({ "", "OBJ", }) );
    set_requirements(REQUIREMENT_BUSY | REQUIREMENT_DISABLE);
}

mixed can_eat () {
    return "Eat what?";
}

void do_eat_obj (object ob, string str) {
    ob->handle_eat(this_character());
}