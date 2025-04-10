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
    /** @type {STD_ROOM} env */
    object env = environment(previous_object());
    if (env->query_property("no attack")) return "You cannot attack here.";
    return 1;
}
/**
 * Attack a living object to engage in combat.
 *
 * @param {STD_LIVING} ob the living object being attacked
 * @param str the input text
 */
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
    initiate_combat(po, ob);
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