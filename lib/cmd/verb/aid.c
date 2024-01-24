inherit STD_VERB;

void create () {
    verb::create();
    add_rules(({ "", "LIV", }));
    set_syntax("aid [target]");
    set_help_text("Your character will attempt to aid the provided target in combat and join combat with present hostiles.");
}

mixed can_aid () {
    return "Aid whom?";
}

mixed can_aid_liv (object lv, string str) {
    if (environment(previous_object())->query_property("no attack")) return "You cannot attack here.";
    return 1;
}
void do_aid_liv (object ob, string str) {
    object po = previous_object();
    object *targets;

    targets = ob->query_present_hostiles();
    if (sizeof(targets)) {
        message("attack", "You run to " + possessive_noun(ob->query_cap_name()) + " aid!", po);
        message("attack", po->query_cap_name() + " runs to your aid!", ob);
        message("attack", po->query_cap_name() + " runs to " + possessive_noun(ob->query_cap_name()) + " aid!", environment(po), ({ po, ob }));
    }

    foreach (object target in ob->query_present_hostiles()) {
        initiate_combat(po, target);
    }
}

void do_aid_lvs (mixed *info, string str) {
    foreach (mixed item in info) {
        if (stringp(item)) {
            write(item + "\n");
        } else {
            do_aid_liv(item, str);
        }
    }
}