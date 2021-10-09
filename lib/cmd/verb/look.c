inherit "/std/verb.c";

void create () {
    verb::create();
    add_rules(({ "", })); // "STR"
    // clear_flag(NEED_TO_SEE);
    // clear_flag(NEED_TO_BE_ALIVE);
}

mixed can_look () {
    return 1;
}
mixed do_look () {
    this_character()->describe_environment();
    return 1;
}

// mixed can_look_str (string str) {
//     if (!environment(this_character())) return "You are nowhere.";
//     // if (this_character()->query_paralyzed()) return "You are unable to move."; // @TODO
//     if (!environment(this_character())->query_exit(str)) return "You cannot go that way.";
//     return 1;
// }
// mixed do_look_str (string str) {
//     write("look "+str+"\n");
//     return 1;
// }