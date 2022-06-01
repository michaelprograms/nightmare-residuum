inherit M_SAVE;

private mapping __Money = ([
/*
    "bankID": ([
        "type": integer,
        ...
    ]),
    ...
*/
]);

/* ----- ----- */

private void clear_balance () {
    __Money = ([ ]);
}
private void load_balance (string name) {
    clear_balance();
    set_save_path(D_CHARACTER->query_save_path(name, "bank"));
    restore_data();
}

/* ----- ----- */

string *query_banks (string name) {
    string *ids;
    load_balance(name);
    ids = keys(__Money);
    clear_balance();

    return ids;
}

mapping query_balance (string name, string bankID) {
    mapping balance;

    if (sizeof(name) < 4) raise_error("Bad argument 1 to bank->query_balance");
    if (!bankID) raise_error("Bad argument 2 to bank->query_balance");

    load_balance(name);
    if (!__Money[bankID]) __Money[bankID] = ([ ]);
    balance = __Money[bankID];
    clear_balance();

    return balance;
}

void update_balance (string name, string bankID, mapping balance) {
    if (sizeof(name) < 4) raise_error("Bad argument 1 to bank->update_balance");
    if (!bankID) raise_error("Bad argument 2 to bank->update_balance");
    if (!mappingp(balance)) raise_error("Bad argument 3 to bank->update_balance");

    load_balance(name);
    __Money[bankID] = balance;
    save_data();
    clear_balance();
}