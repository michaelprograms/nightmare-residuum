#include <verb.h>

inherit STD_VERB;

void create () {
    verb::create();
    add_rules(({ "" }));
    set_requirements(REQUIREMENT_BUSY | REQUIREMENT_DISABLE);
    set_syntax("lay");
    set_help_text("Your character will attempt to lay down if not already in that position.");
}

mixed can_lay () {
    return 1;
}

void do_lay () {
    object tc = this_character();

    if (tc->query_posture() == "laying") {
        message("action", "You are already laying.", tc);
    } else {
        message("action", "You lay down.", tc);
        tc->set_posture("laying");
    }
}