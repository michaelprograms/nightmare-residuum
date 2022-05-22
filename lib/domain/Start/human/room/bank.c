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
    parse_add_rule("balance", "STR");
}

void display_balance (string name, mapping balance, object char) {
    if (undefinedp(balance[__BankDefault])) {
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
    mapping balance = D_BANK->query_balance(this_character()->query_name(), __BankID);
    display_balance(this_character()->query_cap_name(), balance, this_character());
}

mixed can_balance_str (mixed args...) {
    return environment(this_character()) == this_object() && this_character()->query_immortal();
}
void do_balance_str (mixed args...) {
    string name;
    mapping balance;

    if (!arrayp(args) || !sizeof(args) || !(name = args[0])) return;

    if (!D_CHARACTER->query_exists(name)) {
        message("action", "No character found by the name of " + name + ".\n", this_character());
        return;
    }
    balance = D_BANK->query_balance(name, __BankID);
    display_balance(capitalize(name), balance, this_character());
}