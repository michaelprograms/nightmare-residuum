inherit STD_ITEM;
inherit M_CURRENCY;

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

    return "A pile of coins containing of " + conjunction(coins) + ".";
}

void create () {
    ::create();
    set_id(({ "pile", "pile of coins", "coins", "money" }));
    set_name("pile of coins");
    set_short("a pile of coins");
    set_long((: query_long_coin :));
}
