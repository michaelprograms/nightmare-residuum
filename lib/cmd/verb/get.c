inherit STD_VERB;

void create () {
    verb::create();
    add_rules( ({ "", "OBS", }) );
}

mixed can_get () {
    return "Get what?";
}

mixed can_get_obj (object ob, string str) {
    return 1;
}
void do_get_obj (object ob, string str) {
    object po = previous_object();

    message("verb", "You take " + ob->query_name() + ".\n", po);
    message("verb", po->query_cap_name() + " takes " + ob->query_name() + ".\n", environment(ob), ({ po, ob }));
    ob->handle_move(po);
}
void do_get_obs (mixed *info, string str) {
    foreach (mixed item in info) {
        if (stringp(item)) {
            message("verb", item + "\n", previous_object());
        } else {
            do_get_obj(item, str);
        }
    }
}