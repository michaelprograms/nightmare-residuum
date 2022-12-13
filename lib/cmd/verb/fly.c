#include <verb.h>

inherit STD_VERB;

void create () {
    verb::create();
    add_rules(({ "" }));
    set_requirements(REQUIREMENT_BUSY | REQUIREMENT_DISABLE);
    set_syntax("sit");
    set_help_text("Your character will attempt to fly if not already in that position and you have wings or other flight capabilities.");
    set_help_similar(({ "land", "lay", "meditate", "sit", "stand", }));
}

mixed can_fly () {
    return 1;
}

void do_fly () {
    object po = previous_object();
    string *wings, *sWings;

    if (po->query_posture() == "flying") {
        message("action", "You are already flying.", po);
        return;
    }

    sWings = filter(po->query_severed_limbs(), (: regexp($1, "wing$") :));
    if (sizeof(sWings)) {
        message("action", "You cannot fly while missing a wing.", po);
        return;
    }
    wings = filter(po->query_limbs(), (: regexp($1, "wing$") :));
    if (!sizeof(wings)) {
        message("action", "You aren't capable of flying.", po);
        return;
    }

    if (po->query_posture() == "laying") {
        message("action", "You sit up and take to flight.", po);
        message("action", po->query_cap_name() + " sits up and takes to the air in flight.", environment(po), po);
    } else {
        message("action", "You take to the air in flight.", po);
        message("action", po->query_cap_name() + " takes to the air in flight.", environment(po), po);
    }
    po->set_posture("flying");
}