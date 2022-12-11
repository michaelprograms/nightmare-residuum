#include <verb.h>

inherit STD_VERB;

void create () {
    verb::create();
    add_rules(({ "" }));
    set_requirements(REQUIREMENT_BUSY | REQUIREMENT_DISABLE);
    set_syntax("sit");
    set_help_text("Your character will attempt to sit down if not already in that position.");
    set_help_similar(({ "lay", "stand", }));
}

mixed can_sit () {
    return 1;
}

void do_sit () {
    object po = previous_object();

    if (po->query_posture() == "sitting") {
        message("action", "You are already sitting.", po);
        return;
    }

    if (po->query_posture() == "laying") {
        message("action", "You sit up.", po);
        message("action", po->query_cap_name() + " sits up.", environment(po), po);
    } else {
        message("action", "You sit down.", po);
        message("action", po->query_cap_name() + " sits down.", environment(po), po);
    }
    po->set_posture("sitting");
}