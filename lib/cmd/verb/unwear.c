inherit STD_VERB;

void create () {
    verb::create();
    add_rules(({ "", "OBJ", "OBS", }));
}

mixed can_unwear () {
    return "Unwear what?";
}
mixed can_unwear_obj (object ob, string str) {
    // @TODO check if character is busy
    return 1;
}

void do_unwear_obj (object ob, string str) {
    mixed result;
    object po = previous_object();

    if ((result = po->handle_unwear(ob)) == 1) {
        string limbConj = conjunction(ob->query_limbs());
        message("action", "You remove " + ob->query_name() + " from your " + limbConj + ".\n", po);
        message("action", po->query_cap_name() + " removes " + ob->query_name() + " from " + possessive(po) + limbConj + ".\n", environment(po), po);
    } else if (stringp(result)) {
        message("action", result + "\n", po);
    } else {
        message("action", "You try to remove " + ob->query_name() + " but fail for some reason.\n", po);
    }
}
void do_unwear_obs (mixed *info, string str) {
    foreach (object ob in info) {
        do_unwear_obj(ob, str);
    }
}