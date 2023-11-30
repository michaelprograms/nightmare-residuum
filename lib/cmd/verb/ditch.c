inherit STD_VERB;

void create () {
    verb::create();
    add_rules(({ "", "LIV", }));
    set_syntax("ditch [target]");
    set_help_text("Your character will attempt to ditch the provided follower target so that they no longer follow you.");
}

mixed can_ditch () {
    return "Ditch whom?";
}

mixed can_ditch_liv (object lv, string str) {
    return 1;
}
void do_ditch_liv (object ob, string str) {
    object po = previous_object();

    if (!ob->query_following()) {
        message("action", "You are not leading " + ob->query_cap_name() + ".", po);
        return;
    }

    message("action", "You ditch " + ob->query_cap_name() + ".", po);
    message("action", po->query_cap_name() + " ditches you.", ob);
    ob->set_following(0);
}