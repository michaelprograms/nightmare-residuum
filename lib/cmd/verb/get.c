inherit STD_VERB;

void create () {
    verb::create();
    add_rules(({ "", "OBS", "OBJ from OBJ", }));
    set_syntax("get [item] (from [container])");
    set_help_text("Your character will attempt to get the provided input.");
    set_help_similar(({ "drop", "give", "put", }));
}

/* ----- rule: "" ----- */

mixed can_get () {
    return "Get what?";
}

/* ----- rule: "OBS" ----- */

mixed can_get_obj (object ob, string str) {
    return 1;
}
/**
 * Get an item from the living object's environment.
 *
 * @param {STD_ITEM} ob the item being gotten
 * @param str the input text
 */
void do_get_obj (object ob, string str) {
    object po = previous_object();

    message("action", "You take " + ob->query_name() + ".", po);
    message("action", po->query_cap_name() + " takes " + ob->query_name() + ".", environment(po), po);
    ob->handle_move(po);
}
void do_get_obs (mixed *info, string str) {
    foreach (mixed item in info) {
        if (stringp(item)) {
            message("action", item, previous_object());
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

    message("action", "You take " + ob->query_name() + " from " + container->query_name() + ".", po);
    message("action", po->query_cap_name() + " takes " + ob->query_name() + " from " + container->query_name() + ".", environment(po), po);
    ob->handle_move(po);
    return 1;
}