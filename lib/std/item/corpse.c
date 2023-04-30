inherit STD_STORAGE;
inherit M_DUSTABLE;

int is_corpse () { return 1; }

void create () {
    storage::create();
    set_name("corpse");
    set_id(({ "corpse" }));
    set_short("a corpse");
    set_long("A corpse.");
}

void handle_received (object env) {
    storage::handle_received(env);
    dustable::handle_received(env);
}

void setup_body (object ob) {
    string *currencies;

    if (!ob) error("Bad argument 1 to corpse->setup_body");

    set_name("corpse of " + ob->query_name());
    set_id(({ "corpse", "corpse of " + ob->query_short(), "corpse of " + ob->query_name() }));
    set_short("corpse of " + ob->query_short("%^DEFAULT%^"));
    set_long("Here lies the body of " + ob->query_cap_name() + ".");

    if (sizeof(currencies = ob->query_currencies())) {
        object coins = new(STD_COINS);
        foreach (string currency in currencies) {
            coins->add_currency(currency, ob->query_currency(currency));
            ob->add_currency(currency, -ob->query_currency(currency));
        }
        coins->handle_move(this_object());
    }
    foreach (object a in ob->query_all_armor()) {
        a->handle_move(this_object());
    }
    foreach (object w in ob->query_wielded_weapons()) {
        w->handle_move(this_object());
    }
}