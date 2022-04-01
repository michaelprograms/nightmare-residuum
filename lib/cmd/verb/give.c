inherit STD_VERB;

void create () {
    verb::create();
    add_rules( ({ "", "OBS to LIV", "WRD WRD to LIV" }) );
}

mixed can_give () {
    return "Give what to who?";
}

mixed can_give_obj_to_liv (mixed args...) {
    return 1;
}
varargs void do_give_obj_to_liv (object ob, object liv) {
    object tc = this_character();
    message("verb", "You give " + ob->query_name() + " to " +
    liv->query_cap_name() + ".\n", tc);
    message("verb", tc->query_cap_name() + " gives " + ob->query_name() + " to you.\n", liv);
    message("verb", tc->query_cap_name() + " gives " + ob->query_name() + " to " + liv->query_cap_name() + ".\n", environment(tc), ({ tc, liv }));
    return ob->handle_move(liv);
}
void do_give_obs_to_liv (mixed *info, object liv) {
    foreach (mixed item in info) {
        if (stringp(item)) {
            write(item + "\n");
        } else {
            do_give_obj_to_liv(item, liv);
        }
    }
}

mixed can_give_wrd_wrd_to_liv (mixed args...) {
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
mixed do_give_wrd_wrd_to_liv (mixed args...) {
    int amount = to_int(args[0]);
    string currency = args[1];
    object liv = args[2];
    object env = environment(this_character());

    if (!liv) return "You can't give " + amount + " " + currency + " to that."; // default parser response

    liv->add_currency(currency, amount);
    this_character()->add_currency(currency, amount);

    message("verb", "You give " + amount + " " + currency + " to " + liv->query_cap_name() + ".\n", this_character());
    message("verb", this_character()->query_cap_name() + " gives you " + amount + " " + currency + ".\n", liv);
    message("verb", this_character()->query_cap_name() + " gives " + liv->query_cap_name() + " some " + currency + ".\n", env, ({ this_character(), liv }));
    return 1;
}
