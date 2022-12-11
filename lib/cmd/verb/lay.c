#include <verb.h>

inherit STD_VERB;

void create () {
    verb::create();
    add_rules(({ "" }));
    set_requirements(REQUIREMENT_BUSY | REQUIREMENT_DISABLE);
    set_syntax("lay");
    set_help_text("Your character will attempt to lay down if not already in that position.");
    set_help_similar(({ "sit", "stand", }));
}

mixed can_lay () {
    return 1;
}

void do_lay () {
    object po = previous_object();

    if (po->query_posture() == "laying") {
        message("action", "You are already laying.", po);
    } else {
        message("action", "You lay down.", po);
        message("action", po->query_cap_name() + " lays down.", environment(po), po);
        po->set_posture("laying");
    }
}