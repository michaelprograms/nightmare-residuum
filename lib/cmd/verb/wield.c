inherit STD_VERB;

void create () {
    verb::create();
    add_rules( ({ "", "OBJ", }) );
}

mixed can_wield () {
    return "Wield what?";
}

mixed can_wield_obj (object ob, string str) {
    // @TODO check if character is busy
    return 1;
}
void do_wield_obj (object ob, string str) {
    if (ob->query_wielded()) {
        write("You are already wielding " + ob->query_name() + ".\n");
        return;
    }
    if (this_character()->handle_wield(ob)) {
        message("action", "You wield " + ob->query_name() + ".\n", this_character());
        message("action", this_character()->query_cap_name() + " wields " + ob->query_name() + ".\n", environment(this_character()), this_character());
    } else {
        message("action", "You try to wield " + ob->query_name() + " but fail for some reason.\n", this_character());
    }
}