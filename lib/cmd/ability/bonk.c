#include <verb.h>

inherit STD_ABILITY;

void create () {
    ::create();
    set_skill_powers(([
        "melee": 10,
    ]));
    set_help_text("Bonk your opponent with your forehead.");
}

void handle_hit_msg (object source, object target) {
    message("action", "You bonk " + target->query_cap_name() + "!\n", source);
    message("action", source->query_cap_name() + " bonks you!\n", target);
    message("action", source->query_cap_name() + " bonks " + target->query_cap_name() + "!\n", environment(source), ({ source, target }));
}