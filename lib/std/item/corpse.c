inherit STD_STORAGE;

int is_corpse () { return 1; }

void create () {
    storage::create();
    set_name("corpse");
    set_id(({ "corpse" }));
    set_short("a corpse");
    set_long("A corpse.");
}

void handle_expire () {
    if (environment()) {
        message("system", "The " + query_name() + " dissolves into dust.\n", environment());
    }
    remove_call_out();
    handle_remove();
}

void handle_received (object env) {
    ::handle_received(env);
    call_out((: handle_expire :), 60 * 5);
}

void setup_body (object ob) {
    string *currencies;

    if (!ob) error("Bad argument 1 to corpse->setup_body");

    set_name("corpse of " + ob->query_name());
    set_id(({ "corpse", "corpse of " + ob->query_name() }));
    set_short("the corpse of " + ob->query_short("%^DEFAULT%^"));
    set_long("Here lies the body of " + ob->query_cap_name() + ".");

    if (sizeof(currencies = ob->query_currencies())) {
        object coins = new(ITEM_COINS);
        foreach (string currency in currencies) {
            coins->add_currency(currency, ob->query_currency(currency));
            ob->add_currency(currency, -ob->query_currency(currency));
        }
        coins->handle_move(this_object());
    }
}