inherit STD_VERB;

void create () {
    verb::create();
    add_rules(({ "", "OBS", "WRD WRD", }));
    set_syntax("drop [item]");
    set_help_text("Your character will attempt to drop the provided item.");
    set_help_similar(({ "get", "give", "put", }));
}

mixed can_drop () {
    return "Drop what?";
}

mixed can_drop_obj (object ob, string str) {
    return 1;
}
/**
 * Drop an item into the living object's environment.
 *
 * @param {STD_ITEM} ob the item being dropped
 * @param str the input text
 */
void do_drop_obj (object ob, string str) {
    object po = previous_object(), env = environment(po);

    message("action", "You drop " + ob->query_name() + ".", po);
    message("action", po->query_cap_name() + " drops some " + ob->query_name() + ".", env, po);
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
        message("action", "You are unable to drop that.", po);
        coins->handle_remove();
        return 1;
    }
    coins->add_currency(currency, amount);
    message("action", "You drop " + amount + " " + currency + ".", po);
    message("action", po->query_cap_name() + " drops some " + currency + ".", env, po);
    return 1;
}