inherit STD_VERB;

void create () {
    verb::create();
    add_rules(({ "", "OBS", "OBJ from OBJ", }));
    set_syntax("get [item] (from [container])");
    set_help_text("Your character will attempt to get the provided input.");
}

/* ----- rule: "" ----- */

mixed can_get () {
    return "Get what?";
}

/* ----- rule: "OBS" ----- */

mixed can_get_obj (object ob, string str) {
    return 1;
}
void do_get_obj (object ob, string str) {
    object po = previous_object();

    message("verb", "You take " + ob->query_name() + ".\n", po);
    message("verb", po->query_cap_name() + " takes " + ob->query_name() + ".\n", environment(po), po);
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

/* ----- rule: "OBJ from OBJ" ----- */

mixed can_get_obj_from_obj (mixed args...) {
    return 1;
}

mixed do_get_obj_from_obj (mixed args...) {
    object po = previous_object();
    object ob = args[0], container = args[1];

    message("verb", "You take " + ob->query_name() + " from " + container->query_name() + ".\n", po);
    message("verb", po->query_cap_name() + " takes " + ob->query_name() + " from " + container->query_name() + ".\n", environment(po), po);
    ob->handle_move(po);
    return 1;
}