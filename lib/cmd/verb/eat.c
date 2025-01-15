#include <verb.h>

inherit STD_VERB;

void create () {
    verb::create();
    add_rules(({ "", "OBJ", }));
    set_requirements(REQUIREMENT_BUSY | REQUIREMENT_DISABLE);
    set_syntax("eat [item]");
    set_help_text("Your character will attempt to eat the provided item.");
    set_help_similar(({ "drink", }));
}

mixed can_eat () {
    return "Eat what?";
}

/**
 * Handle consuming a food object.
 *
 * @param {STD_FOOD} ob the object to eat
 * @param str the input text
 */
void do_eat_obj (object ob, string str) {
    ob->handle_consume(this_character());
}