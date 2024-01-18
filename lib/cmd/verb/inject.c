#include <verb.h>

inherit STD_VERB;

void create () {
    verb::create();
    add_rules(({ "", "OBJ", }));
    set_requirements(REQUIREMENT_BUSY | REQUIREMENT_DISABLE);
    set_syntax("inject [item]");
    set_help_text("Your character will attempt to inject the provided item.");
    set_help_similar(({ "inject", }));
}

mixed can_inject () {
    return "Inject what?";
}

void do_inject_obj (object ob, string str) {
    ob->handle_inject(this_character());
}