inherit "/std/verb.c";

void create () {
    verb::create();
    add_rules(({ "", "STR" }));
}

mixed can_go () {
    return "Go where?";
}

mixed can_go_str (string str) {
    if (!environment(this_character())) return "You are nowhere.";
    // if (this_character()->query_paralyzed()) return "You are unable to move."; // @TODO
    if (!environment(this_character())->query_exit(str)) return "You cannot go that way.";
    return 1;
}
mixed do_go_str (string str) {
    return environment(previous_object())->handle_go(previous_object(), str);
}