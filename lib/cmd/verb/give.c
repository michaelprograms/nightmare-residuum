inherit STD_VERB;

void create () {
    verb::create();
    add_rules(({ "", "OBS to LIV", "WRD WRD to LIV" }));
    set_syntax("give [item] to [target]");
    set_help_text("Your character will attempt to give the provided item to the provided target.");
    set_help_similar(({ "drop", "get", "put", }));
}

mixed can_give () {
    return "Give what to who?";
}

mixed can_give_obj_to_liv (mixed args...) {
    return 1;
}
varargs void do_give_obj_to_liv (object ob, object liv) {
    object tc = this_character();
    message("action", "You give " + ob->query_name() + " to " +
    liv->query_cap_name() + ".", tc);
    message("action", tc->query_cap_name() + " gives " + ob->query_name() + " to you.", liv);
    message("action", tc->query_cap_name() + " gives " + ob->query_name() + " to " + liv->query_cap_name() + ".", environment(tc), ({ tc, liv }));
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

    if (amount < 1) {
        return 0;
    }
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

    if (!liv) {
        return "You can't give " + amount + " " + currency + " to that."; // default parser response
    }

    liv->add_currency(currency, amount);
    this_character()->add_currency(currency, amount);

    message("action", "You give " + amount + " " + currency + " to " + liv->query_cap_name() + ".", this_character());
    message("action", this_character()->query_cap_name() + " gives you " + amount + " " + currency + ".", liv);
    message("action", this_character()->query_cap_name() + " gives " + liv->query_cap_name() + " some " + currency + ".", env, ({ this_character(), liv }));
    return 1;
}
