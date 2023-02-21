inherit STD_VERB;

void create () {
    verb::create();
    add_rules(({ "", "STR" }));
    set_syntax("climb [direction]");
    set_help_text("Your character will attempt to climb in the provided direction.");
}

mixed can_climb () {
    return "Climb where?";
}

varargs mixed can_climb_str (string dir) {
    object po = previous_object(), env;

    if (!(env = environment(po))) return "You are nowhere.";
    if (!env->query_climb(dir)) return "You cannot climb that way.";
    if (po->query_posture() != "standing") return "You cannot climb in that position.";

    return 1;
}
varargs void do_climb_str (string dir) {
    object po = previous_object(), env = environment(po);

    if (po->query_posture() != "standing") {
        po->do_command("stand");
    }

    env->handle_climb(po, "climb", dir);
}