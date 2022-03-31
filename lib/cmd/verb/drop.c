inherit STD_VERB;

void create () {
    verb::create();
    add_rules( ({ "", "OBS", "WRD WRD", }) );
}

mixed can_drop () {
    return "Drop what?";
}

mixed can_drop_obj (object ob, string str) {
    return 1;
}
void do_drop_obj (object ob, string str) {
    write("You drop " + ob->query_name() + ".\n");
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
    int amount = to_int(args[0]), n;
    string currency = args[1];

    if (amount < 1) return 0;
    if ((n = this_character()->query_currency(currency)) < 1) {
        return "You do not have any " + currency + " to drop.";
    }
    if (n < amount) {
        return "You do not have enough " + currency + " to drop that much.";
    }
    return 1;
}
mixed do_drop_wrd_wrd (mixed args...) {
    int amount = to_int(args[0]);
    string currency = args[1];
    object env = environment(this_character());
    object coins;

    coins = new("/std/item/coins.c");
    if (!coins->handle_move(env) || !this_character()->add_currency(currency, -amount)) {
        message("verb", "You are unable to drop that.\n", this_character());
        coins->handle_remove();
        return 1;
    }
    coins->add_currency(currency, amount);
    message("verb", "You drop " + amount + " " + currency + ".\n", this_character());
    message("verb", this_character()->query_cap_name() + " drops some " + currency + ".\n", env, this_character());
    return 1;
}