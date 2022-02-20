inherit STD_VERB;

void create () {
    verb::create();
    add_rules( ({ "", "OBJ", }) );
}

mixed can_eat () {
    return "Eat what?";
}

mixed can_eat_obj (object ob, string str) {
    // @TODO check if character is busy
    return 1;
}
void do_eat_obj (object ob, string str) {
    ob->handle_eat(this_character());
}