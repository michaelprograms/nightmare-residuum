inherit STD_VERB;

void create () {
    verb::create();
    add_rules( ({ "", "OBS", }) ); // @TODO if currency "WRD STR" }));
}

mixed can_drop () {
    return "Drop what?";
}

mixed can_drop_obj (object ob, string str) {
    return 1;
}
void do_drop_obj (object ob, string str) {
    write("You drop " + ob->query_name() + ".\n");
    ob->handle_move(environment(this_character()));
}
void do_drop_obs (mixed *info, string str) {
    foreach (mixed item in info) {
        if (stringp(item)) {
            write(item + "\n");
        } else {
            do_drop_obj(item, str);
        }
    }
}

// @TODO if currency
// varargs mixed can_drop_wrd_str (string word, string str) {
//     write("can_drop_wrd_str " + word + " " + str + "\n");
// }
// varargs mixed do_drop_wrd_str (string word, string str) {
//     write("do_drop_wrd_str " + word + " " + str + "\n");
// }