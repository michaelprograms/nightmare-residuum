#include <verb.h>

inherit STD_ABILITY;

void create () {
    ::create();
    set_requirements(REQUIREMENT_BUSY | REQUIREMENT_DISABLE);
    set_skill_powers(([
        "psionic": 8,
        "ranged": 2,
    ]));
}

void handle_hit_msg (object source, object target) {
    message("action", "You zap " + target->query_cap_name() + "!\n", source);
    message("action", source->query_cap_name() + " zaps you!\n", target);
    message("action", source->query_cap_name() + " zaps " + target->query_cap_name() + "!\n", environment(source), ({ source, target }));
}

void help (object char) {
    message("help", "Zap your opponent with your mind.\n", char);
}