inherit STD_VERB;

void create () {
    verb::create();
    add_rules(({ "", "STR" }));
}

mixed can_go () {
    return "Go where?";
}

varargs mixed can_go_str (string dir) {
    object env;
    string exit;

    if (!(env = environment(previous_object()))) return "You are nowhere.";
    if (dir == "enter") {
        if (exit = env->query_default_enter()) dir = exit;
        else return "Go enter which way?";
    } else if (dir == "out") {
        if (exit = env->query_default_out()) dir = exit;
        else return "Go out which way?";
    }
    if (!env->query_exit(dir)) return "You cannot go that way.";
    return 1;
}
varargs void do_go_str (string dir) {
    object env;
    string exit;

    if (!(env = environment(previous_object()))) return "You are nowhere.";

    if (dir == "enter") {
        if (exit = env->query_default_enter()) dir = exit;
        else return "Enter which way?";
    } else if (dir == "out") {
        if (exit = env->query_default_out()) dir = exit;
        else return "Go out which way?";
    }
    environment(previous_object())->handle_go(previous_object(), "walk", dir);
}