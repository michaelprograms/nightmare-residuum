#include <verb.h>

inherit STD_ABILITY;

void create () {
    ::create();
    set_requirements(REQUIREMENT_BUSY | REQUIREMENT_DISABLE);
    set_skill_powers(([
        "melee": 10,
        "psionic": 5,
    ]));
}

void handle_hit_msg (object source, object target) {
    message("action", "You smite " + target->query_cap_name() + "!\n", source);
    message("action", source->query_cap_name() + " smites you!\n", target);
    message("action", source->query_cap_name() + " smites " + target->query_cap_name() + "!\n", environment(source), ({ source, target }));
}