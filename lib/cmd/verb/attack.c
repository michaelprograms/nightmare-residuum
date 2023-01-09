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
    message("action", "%^RED%^BOLD%^You attack " + ob->query_cap_name() + "!%^RESET%^", previous_object());
    message("action", "%^RED%^BOLD%^" + previous_object()->query_cap_name() + " attacks you!%^RESET%^", ob);
    message("action", "%^RED%^BOLD%^" + previous_object()->query_cap_name() + " attacks " + ob->query_cap_name() + "!%^RESET%^", environment(previous_object()), previous_object());

    previous_object()->add_hostile(ob);
    ob->add_hostile(previous_object());
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