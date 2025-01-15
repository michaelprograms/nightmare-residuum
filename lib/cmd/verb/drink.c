#include <verb.h>

inherit STD_VERB;

void create () {
    verb::create();
    add_rules(({ "", "OBJ", }));
    set_requirements(REQUIREMENT_BUSY | REQUIREMENT_DISABLE);
    set_syntax("drink [item]");
    set_help_text("Your character will attempt to drink the provided item.");
    set_help_similar(({ "drink", }));
}

mixed can_drink () {
    return "Drink what?";
}

/**
 * Handle consuming a drink object.
 *
 * @param {STD_DRINK} ob the object to drink
 * @param str the input text
 */
void do_drink_obj (object ob, string str) {
    ob->handle_consume(this_character());
}