inherit STD_VERB;

void create () {
    verb::create();
    add_rules(({ "", "STR" }));
}

mixed can_go () {
    return "Go where?";
}

mixed can_go_str (string str) {
    object env;
    string exit;

    if (!(env = environment(previous_object()))) return "You are nowhere.";
    if (str == "enter") {
        if (exit = env->query_default_enter()) str = exit;
        else return "Go enter which way?";
    } else if (str == "out") {
        if (exit = env->query_default_out()) str = exit;
        else return "Go out which way?";
    }
    if (!env->query_exit(str)) return "You cannot go that way.";
    return 1;
}
mixed do_go_str (string str) {
    object env;
    string exit;

    if (!(env = environment(previous_object()))) return "You are nowhere.";

    if (str == "enter") {
        if (exit = env->query_default_enter()) str = exit;
        else return "Enter which way?";
    } else if (str == "out") {
        if (exit = env->query_default_out()) str = exit;
        else return "Go out which way?";
    }
    return environment(previous_object())->handle_go(previous_object(), "walk", str);
}