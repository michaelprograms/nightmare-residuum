inherit STD_VERB;

void create () {
    verb::create();
    add_rules(({ "", "LVS", }), ({ "kill", "battle" }));
}

mixed can_attack () {
    return "Attack whom?";
}

mixed can_attack_liv (object lv, string str) {
    return 1;
}
void do_attack_liv (object ob, string str) {
    message("action", "%^RED%^BOLD%^You attack " + ob->query_name() + "!%^RESET%^\n", this_character());
    message("action", "%^RED%^BOLD%^" + this_character()->query_name() + " attacks " + ob->query_name() + "!%^RESET%^\n", environment(this_character()), this_character());

    this_character()->add_hostile(ob);
    ob->add_hostile(this_character());
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