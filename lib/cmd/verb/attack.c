inherit STD_VERB;

void create () {
    verb::create();
    add_rules(({ "", "LVS", }), ({ "kill", "battle" }));
    set_syntax("attack [targets]");
    set_help_text("Your character will attempt to attack the provided targets.");
}

mixed can_attack () {
    return "Attack whom?";
}

mixed can_attack_liv (object lv, string str) {
    if (environment(previous_object())->query_property("no attack")) return "You cannot attack here.";
    return 1;
}
void do_attack_liv (object ob, string str) {
    object po = previous_object();
    if (po->query_defeated()) {
        message("action", "You cannot attack someone while you are defeated.", po);
        return;
    }
    if (ob && ob->query_defeated()) {
        message("action", "You cannot attack someone who is already defeated.", po);
        return;
    }
    message("attack", "You attack " + ob->query_cap_name() + "!", po);
    message("attack", po->query_cap_name() + " attacks you!", ob);
    message("attack", po->query_cap_name() + " attacks " + ob->query_cap_name() + "!", environment(po), po);

    po->add_hostile(ob);
    ob->add_hostile(po);
}
void do_attack_lvs (mixed *info, string str) {
    foreach (mixed item in info) {
        if (stringp(item)) {
            write(item + "\n");
        } else {
            do_attack_liv(item, str);
        }
    }
}