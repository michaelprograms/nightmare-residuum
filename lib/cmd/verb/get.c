inherit STD_VERB;

void create () {
    verb::create();
    add_rules( ({ "", "OBS", }) ); // @TODO if currency "WRD STR" }));
}

mixed can_get () {
    return "Get what?";
}

mixed can_get_obj (object ob, string str) {
    return 1;
}
mixed do_get_obj (object ob, string str) {
    write("You get " + ob->query_name() + ".\n");
    return ob->handle_move(this_character());
}
mixed do_get_obs (mixed *info, string str) {
    foreach (mixed item in info) {
        if (stringp(item)) {
            write(item + "\n");
        } else {
            write("You get " + item->query_name() + ".\n");
            item->handle_move(this_character());
        }
    }
    return 1;
}

// @TODO if currency
// varargs mixed can_get_wrd_str (string word, string str) {
//     write("can_get_wrd_str " + word + " " + str + "\n");
// }
// varargs mixed do_get_wrd_str (string word, string str) {
//     write("do_get_wrd_str " + word + " " + str + "\n");
// }