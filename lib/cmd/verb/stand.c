#include <verb.h>

inherit STD_VERB;

void create () {
    verb::create();
    add_rules(({ "" }));
    set_requirements(REQUIREMENT_BUSY | REQUIREMENT_DISABLE);
    set_syntax("stand");
    set_help_text("Your character will attempt to stand up if not already in that position.");
    set_help_similar(({ "fly", "land", "lay", "meditate", "sit", }));
}

mixed can_stand () {
    return 1;
}

void do_stand () {
    object po = previous_object();

    if (po->query_posture() == "standing") {
        message("action", "You are already standing.", po);
        return;
    } else {
        message("action", "You stand up.", po);
        message("action", po->query_cap_name() + " stands up.", environment(po), po);
        po->set_posture("standing");
    }
}