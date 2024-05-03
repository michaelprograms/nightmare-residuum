inherit STD_ITEM;
inherit M_CURRENCY;

int is_currency () {
    return 1;
}

string query_long_coin () {
    string *coins = ({ });
    int n;

    foreach (string currency in query_currencies()) {
        if (n = query_currency(currency)) {
            coins += ({ n + " " + currency });
        }
    }

    if (!sizeof(coins)) {
        coins += ({ "coins of no value" });
    }

    return "A pile of coins consisting of " + conjunction(coins) + ".";
}

void create () {
    ::create();
    set_id(({ "pile", "pile of coins", "coins", "money" }));
    set_name("pile of coins");
    set_short("a pile of coins");
    set_long((: query_long_coin :));
}

void check_empty () {
    foreach (string currency in query_currencies()) {
        if (query_currency(currency)) {
            return;
        }
    }
    handle_remove();
}

void handle_received (object env) {
    string *list = ({ });

    ::handle_received(env);

    if (env && inherits(M_CURRENCY, env)) {
        foreach (string c in query_currencies()) {
            int n = query_currency(c);
            env->add_currency(c, n);
            add_currency(c, -n);
            list += ({ n + " " + c });
        }
        if (sizeof(list)) {
            message("action", "You receive " + conjunction(list) + ".", env);
        }
        check_empty();
    }
}