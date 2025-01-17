inherit STD_VERB;

void create () {
    verb::create();
    add_rules(({ "", "LIV", }));
    set_syntax("lead [target]");
    set_help_text("Your character will attempt to lead the provided follower target so that they begin to follow you.");
}

mixed can_lead () {
    return "Lead whom?";
}

mixed can_lead_liv (object lv, string str) {
    return 1;
}

/**
 * Parser apply for leading a living object such as a pet.
 *
 * @param {"/std/npc/pet.c"} ob the living object being lead
 * @param str
 */
void do_lead_liv (object ob, string str) {
    object po = previous_object();

    if (ob->query_following()) {
        message("action", "You are already leading " + ob->query_cap_name() + ".", po);
        return;
    }

    message("action", "You begin to lead " + ob->query_cap_name() + ".", po);
    message("action", po->query_cap_name() + " begins to lead you.", ob);
    ob->set_following(1);
}