#include <verb.h>

inherit STD_ABILITY;

void create () {
    ::create();
    set_requirements(REQUIREMENT_BUSY | REQUIREMENT_DISABLE);
    set_skill_type("melee");
    set_base_power(10);
}

void handle_hit_msg (object source, object target) {
    message("action", "You bonk " + target->query_cap_name() + "!\n", source);
    message("action", source->query_cap_name() + " bonks you!\n", target);
    message("action", source->query_cap_name() + " bonks " + target->query_cap_name() + "!\n", environment(source), ({ source, target }));
}