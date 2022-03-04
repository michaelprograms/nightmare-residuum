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
    this_character()->handle_wield(ob);
}