inherit STD_VERB;

void create () {
    verb::create();
    add_rules( ({ "", "OBJ", }) );
}

mixed can_unwield () {
    return "Unwield what?";
}

mixed can_unwield_obj (object ob, string str) {
    // @TODO check if character is busy
    return 1;
}
void do_unwield_obj (object ob, string str) {
    if (!ob->query_wielded()) {
        write("You are already not wielding " + ob->query_name() + ".\n");
        return;
    }
    if (this_character()->handle_unwield(ob)) {
        message("action", "You unwield " + ob->query_name() + ".\n", this_character());
        message("action", "You unwield " + ob->query_name() + ".\n", environment(this_character()), this_character());
    } else {
        message("action", "You try to unwield " + ob->query_name() + " but fail for some reason.\n", this_character());
    }
}