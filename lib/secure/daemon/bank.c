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

private void load_balance (string name) {
    __Money = ([ ]);
    set_save_path(D_CHARACTER->query_save_path(name, "bank"));
    restore_data();
}

/* ----- ----- */

mapping query_balance (string name, string bankID) {
    mapping balance;

    if (strlen(name) < 4) error("Bad argument 1 to bank->query_balance");
    if (undefinedp(bankID)) error("Bad argument 2 to bank->query_balance");

    load_balance(name);
    if (undefinedp(__Money[bankID])) __Money[bankID] = ([ ]);
    balance = __Money[bankID];
    __Money = ([ ]);

    return balance;
}

void update_balance (string name, string bankID, mapping balance) {
    if (strlen(name) < 4) error("Bad argument 1 to bank->update_balance");
    if (undefinedp(bankID)) error("Bad argument 2 to bank->update_balance");
    if (!mapp(balance)) error("Bad argument 3 to bank->update_balance");

    load_balance(name);
    __Money[bankID] = balance;
    save_data();
    __Money = ([ ]);
}