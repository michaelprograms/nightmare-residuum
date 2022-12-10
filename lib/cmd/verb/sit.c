#include <verb.h>

inherit STD_VERB;

void create () {
    verb::create();
    add_rules(({ "" }));
    set_requirements(REQUIREMENT_BUSY | REQUIREMENT_DISABLE);
    set_syntax("sit");
    set_help_text("Your character will attempt to sit down if not already in that position.");
}

mixed can_sit () {
    return 1;
}

void do_sit () {
    object tc = this_character();

    if (tc->query_posture() == "sitting") {
        message("action", "You are already sitting.", tc);
        return;
    }

    if (tc->query_posture() == "laying") {
        message("action", "You sit up.", tc);
    } else {
        message("action", "You sit down.", tc);
    }
    tc->set_posture("sitting");
}