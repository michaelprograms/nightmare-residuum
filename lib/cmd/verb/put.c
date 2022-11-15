inherit STD_VERB;

void create () {
    verb::create();
    add_rules(({ "", "OBJ in OBJ", }));
    set_syntax("put [item] in [container]");
    set_help_text("Your character will attempt to put the provided item into the provided container.");
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

    message("verb", "You put " + ob->query_name() + " into " + container->query_name() + ".\n", po);
    message("verb", po->query_cap_name() + " puts " + ob->query_name() + " into " + container->query_name() + ".\n", environment(po), po);
    ob->handle_move(container);
    return 1;
}