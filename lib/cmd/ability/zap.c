#include <verb.h>

inherit STD_ABILITY;

void create () {
    ::create();
    set_requirements(REQUIREMENT_BUSY | REQUIREMENT_DISABLE);
    set_skill_type("psionic");
    set_base_power(10);
}

mixed can_zap () {
    return "Zap whom?";
}
void do_zap (mixed args...) {
    // @TODO find current attacker and handle
    write("Zap! args: " + identify(args) + "\n");
}

void handle_hit_msg (object source, object target) {
    message("action", "You zap " + target->query_cap_name() + "!\n", source);
    message("action", source->query_cap_name() + " zaps you!\n", target);
    message("action", source->query_cap_name() + " zaps " + target->query_cap_name() + "!\n", environment(source), ({ source, target }));
}