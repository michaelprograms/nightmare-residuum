#include <verb.h>

inherit STD_VERB;

void create () {
    verb::create();
    add_rules(({ "" }));
    set_requirements(REQUIREMENT_BUSY | REQUIREMENT_DISABLE);
    set_syntax("sit");
    set_help_text("Your character will attempt to land if flying in the air.");
    set_help_similar(({ "fly", "lay", "meditate", "sit", "stand", }));
}

mixed can_fly () {
    return 1;
}

void do_fly () {
    object po = previous_object();

    if (po->query_posture() == "flying") {
        message("action", "You land on your feet.", po);
        message("action", po->query_cap_name() " lands on " + possessive(po) + " feet.", po);
        po->set_posture("standing");
        return;
    } else {
        message("action", "You are not flying.", po);
    }
}