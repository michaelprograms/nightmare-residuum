inherit STD_VERB;

void create () {
    verb::create();
    add_rules(({ "", "OBJ", "OBS", }));
}

mixed can_unwield () {
    return "Unwield what?";
}
mixed can_unwield_obj (object ob, string str) {
    // @TODO check if character is busy
    return 1;
}

void do_unwield_obj (object ob, string str) {
    mixed result;
    object po = previous_object();
    string limbConj = conjunction(po->query_wielded_limbs(ob));

    if ((result = po->handle_unwield(ob)) == 1) {
        message("action", "You unwield " + ob->query_name() + " from your " + limbConj + ".\n", po);
        message("action", po->query_cap_name() + " unwields " + ob->query_name() + " from " + possessive(po) + limbConj + ".\n", environment(po), po);
    } else if (stringp(result)) {
        message("action", result + "\n", po);
    } else {
        message("action", "You try to unwield " + ob->query_name() + " but fail for some reason.\n", po);
    }
}
void do_unwield_obs (mixed *info, string str) {
    foreach (object ob in info) {
        do_unwield_obj(ob, str);
    }
}