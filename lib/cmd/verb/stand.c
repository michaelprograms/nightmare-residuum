#include <verb.h>

inherit STD_VERB;

void create () {
    verb::create();
    add_rules(({ "" }));
    set_requirements(REQUIREMENT_BUSY | REQUIREMENT_DISABLE);
    set_syntax("stand");
    set_help_text("Your character will attempt to stand up if not already in that position.");
}

mixed can_stand () {
    return 1;
}

void do_stand () {
    object tc = this_character();

    if (tc->query_posture() == "standing") {
        message("action", "You are already standing.", tc);
        return;
    } else {
        message("action", "You stand up.", tc);
        tc->set_posture("standing");
    }
}