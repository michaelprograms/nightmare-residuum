inherit STD_VERB;

void create () {
    verb::create();
    add_rules(({ "", "OBJ", "OBS", }));
}

mixed can_wear () {
    return "Wear what?";
}
mixed can_wear_obj (object ob, string str) {
    // @TODO check if character is busy
    return 1;
}

void do_wear_obj (object ob, string str) {
    mixed result;
    object po = previous_object();

    if ((result = po->handle_wear(ob)) == 1) {
        string limbConj = conjunction(ob->query_limbs());
        message("action", "You wear " + ob->query_name() + " on your " + limbConj + ".\n", po);
        message("action", po->query_cap_name() + " wears " + ob->query_name() + " on " + possessive(po) + limbConj + ".\n", environment(po), po);
    } else if (stringp(result)) {
        message("action", result + "\n", po);
    } else {
        message("action", "You try to wear " + ob->query_name() + " but fail for some reason.\n", po);
    }
}
void do_wear_obs (mixed *info, string str) {
    foreach (object ob in info) {
        do_wear_obj(ob, str);
    }
}