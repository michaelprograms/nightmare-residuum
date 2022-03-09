inherit STD_VERB;

void create () {
    verb::create();
    add_rules( ({ "", "OBS to LIV", }) ); // @TODO if currency "WRD STR to LIV" }));
}

mixed can_give () {
    return "Give what to who?";
}

mixed can_give_obj_to_liv (mixed ob, mixed liv, string str, mixed d) {
    return 1;
}
varargs void do_give_obj_to_liv (object ob, object liv) {
    object tc = this_character();
    message("verb", "You give " + ob->query_name() + " to " +
    liv->query_cap_name() + ".\n", tc);
    message("verb", tc->query_cap_name() + " gives " + ob->query_name() + " to you.\n", liv);
    message("verb", tc->query_cap_name() + " gives " + ob->query_name() + " to " + liv->query_cap_name() + ".\n", environment(tc), ({ tc, liv }));
    return ob->handle_move(liv);
}
void do_give_obs_to_liv (mixed *info, object liv) {
    foreach (mixed item in info) {
        if (stringp(item)) {
            write(item + "\n");
        } else {
            do_give_obj_to_liv(item, liv);
        }
    }
}

// @TODO if currency
// varargs mixed can_give_wrd_str (string word, string str) {
//     write("can_give_wrd_str " + word + " " + str + "\n");
// }
// varargs mixed do_give_wrd_str (string word, string str) {
//     write("do_give_wrd_str " + word + " " + str + "\n");
// }