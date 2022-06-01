#include "human.h"

inherit STD_ROOM;

nosave private string __BankID = "start";
nosave private string __BankDefault = "copper";

void create () {
    ::create();
    set_short("a village bank");
    set_long("The inside of a brick building that consists of a large open room. There is a large glass window to the north facing the street with a door to the side. There is a vault towards the south of the building.");
    set_exits(([
        "out north": HUMAN_ROOM "market_st_w1.c",
    ]));
    set_looks(([
        "vault": "A closed and secured vault door with no way to see through.",
    ]));

    parse_init();
    parse_add_rule("balance", "");
    parse_add_rule("balance", "WRD");
    parse_add_rule("deposit", "WRD WRD");
    parse_add_rule("withdraw", "WRD WRD");
}

/* ----- balance ----- */

void display_balance (string name, mapping balance, object char) {
    if (!balance[__BankDefault]) {
        balance[__BankDefault] = 0;
    }
    message("action", "Account balance for " + name + ":\n", char);
    foreach (string key,int value in balance) {
        message("action", "  " + capitalize(key) + ": "+value+"\n", char);
    }
}

mixed can_balance () {
    return environment(this_character()) == this_object();
}
void do_balance () {
    object tc = this_character();
    mapping balance = D_BANK->query_balance(tc->query_name(), __BankID);
    message("action", "You query your bank account information...\n", tc);
    message("action", tc->query_cap_name() + " queries " + possessive(tc) + " bank account information.\n", environment(tc), tc);
    display_balance(tc->query_cap_name(), balance, tc);
}

mixed can_balance_wrd (mixed args...) {
    return environment(this_character()) == this_object() && previous_object()->query_immortal();
}
void do_balance_wrd (mixed args...) {
    string name;
    mapping balance;

    if (!pointerp(args) || !sizeof(args) || !(name = args[0])) return;

    if (!D_CHARACTER->query_exists(name)) {
        message("action", "No character found by the name of " + name + ".\n", this_character());
        return;
    }
    balance = D_BANK->query_balance(name, __BankID);
    display_balance(capitalize(name), balance, this_character());
}

/* ----- ----- */

mixed can_deposit_wrd_wrd (mixed args...) {
    return environment(this_character()) == this_object();
}
void do_deposit_wrd_wrd (mixed args...) {
    string currency;
    int amount;
    object tc = this_character();
    mapping balance;

    if (!pointerp(args) || !sizeof(args) || !(amount = to_int(args[0])) || !(currency = args[1])) return;

    if (member(currency, tc->query_currencies()) == -1) {
        message("action", "You have no currency named " + currency + ".\n", tc);
        return;
    }
    if (amount < 1 || tc->query_currency(currency) < amount) {
        message("action", "You do not have enough " + currency + " to deposit.\n", tc);
        return;
    }

    balance = D_BANK->query_balance(tc->query_name(), __BankID);
    tc->add_currency(currency, -amount);
    balance[currency] += amount;
    D_BANK->update_balance(tc->query_name(), __BankID, balance);

    message("action", "You deposit " + amount + " " + currency + ".\n", tc);
    message("action", tc->query_cap_name() + " deposits some " + currency + ".\n", environment(tc), tc);
}

/* ----- ----- */

mixed can_withdraw_wrd_wrd (mixed args...) {
    return environment(this_character()) == this_object();
}
void do_withdraw_wrd_wrd (mixed args...) {
    string currency;
    int amount;
    object tc = this_character();
    mapping balance;

    if (!pointerp(args) || !sizeof(args) || !(amount = to_int(args[0])) || !(currency = args[1])) return;

    balance = D_BANK->query_balance(tc->query_name(), __BankID);

    if (member(currency, keys(balance)) == -1) {
        message("action", "Your account has no currency named " + currency + ".\n", tc);
        return;
    }
    if (amount < 1 || balance[currency] < amount) {
        message("action", "Your account does not have enough " + currency + " to withdraw.\n", tc);
        return;
    }

    tc->add_currency(currency, amount);
    balance[currency] += -amount;
    D_BANK->update_balance(tc->query_name(), __BankID, balance);

    message("action", "You withdraw " + amount + " " + currency + ".\n", tc);
    message("action", tc->query_cap_name() + " withdraws some " + currency + ".\n", environment(tc), tc);
}