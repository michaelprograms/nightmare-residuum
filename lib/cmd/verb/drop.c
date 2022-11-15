inherit STD_VERB;

void create () {
    verb::create();
    add_rules(({ "", "OBS", "WRD WRD", }));
    set_syntax("drop [item]");
    set_help_text("Your character will attempt to drop the provided item.");
}

mixed can_drop () {
    return "Drop what?";
}

mixed can_drop_obj (object ob, string str) {
    return 1;
}
void do_drop_obj (object ob, string str) {
    object po = previous_object(), env = environment(po);

    message("verb", "You drop " + ob->query_name() + ".\n", po);
    message("verb", po->query_cap_name() + " drops some " + ob->query_name() + ".\n", env, po);
    ob->handle_move(environment(this_character()));
}
void do_drop_obs (mixed *info, string str) {
    foreach (mixed item in info) {
        if (stringp(item)) {
            write(item + "\n");
        } else {
            do_drop_obj(item, str);
        }
    }
}

mixed can_drop_wrd_wrd (mixed args...) {
    object po = previous_object();
    int amount = to_int(args[0]), n;
    string currency = args[1];

    if (amount < 1) return 0;
    if ((n = po->query_currency(currency)) < 1) {
        return "You do not have any " + currency + " to drop.";
    }
    if (n < amount) {
        return "You do not have enough " + currency + " to drop that much.";
    }
    return 1;
}
mixed do_drop_wrd_wrd (mixed args...) {
    object po = previous_object(), env = environment(po);
    int amount = to_int(args[0]);
    string currency = args[1];
    object coins;

    coins = new("/std/item/coins.c");
    if (!coins->handle_move(env) || !po->add_currency(currency, -amount)) {
        message("verb", "You are unable to drop that.\n", po);
        coins->handle_remove();
        return 1;
    }
    coins->add_currency(currency, amount);
    message("verb", "You drop " + amount + " " + currency + ".\n", po);
    message("verb", po->query_cap_name() + " drops some " + currency + ".\n", env, po);
    return 1;
}