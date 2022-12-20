inherit STD_VERB;

void create () {
    verb::create();
    add_rules(({ "", "OBJ in OBJ", }));
    set_syntax("put [item] in [container]");
    set_help_text("Your character will attempt to put the provided item into the provided container.");
    set_help_similar(({ "drop", "get", "give", }));
}

/* ----- rule: "" ----- */

mixed can_put () {
    return "Put what?";
}

/* ----- rule: "OBJ in OBJ" ----- */

mixed can_put_obj_in_obj (mixed args...) {
    return 1;
}

mixed do_put_obj_in_obj (mixed args...) {
    object po = previous_object();
    object ob = args[0], container = args[1];

    message("action", "You put " + ob->query_name() + " into " + container->query_name() + ".", po);
    message("action", po->query_cap_name() + " puts " + ob->query_name() + " into " + container->query_name() + ".", environment(po), po);
    ob->handle_move(container);
    return 1;
}