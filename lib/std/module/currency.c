#define VALID_CURRENCIES ({ "copper" })

mapping __Currency;

private void initialize_currencies () {
    if (!mapp(__Currency)) {
        __Currency = ([ ]);
    }
}

string *query_currencies () {
    initialize_currencies();
    return keys(filter(__Currency, (: $2 > 0 :)));
}

int query_currency (string type) {
    initialize_currencies();
    if (!stringp(type)) {
        error("Bad argument 1 to currency->query_currency");
    }
    if (member_array(type, VALID_CURRENCIES) == -1) {
        return -1;
    }
    if (undefinedp(__Currency[type])) {
        return 0;
    } else {
        return __Currency[type];
    }
}

int add_currency (string type, int amount) {
    int result = 0;

    initialize_currencies();

    if (!stringp(type) || member_array(type, VALID_CURRENCIES) == -1) {
        error("Bad argument 1 to currency->add_currency");
    }
    if (undefinedp(amount) || !intp(amount)) {
        error("Bad argument 2 to currency->add_currency");
    }

    if (undefinedp(__Currency[type])) {
        __Currency[type] = 0;
    }

    if (amount > 0 || __Currency[type] >= abs(amount)) {
        __Currency[type] += amount;
        result = 1;
    }

    if (__Currency[type] < 1) {
        map_delete(__Currency, type);
    }

    return result;
}