#define VALID_CURRENCIES ({ "copper" })

private mapping __Currency;

string *query_currencies () {
    if (!mappingp(__Currency)) __Currency = ([ ]);
    return keys(filter(__Currency, (: $2 > 0 :)));
}

int query_currency (string type) {
    if (!mappingp(__Currency)) __Currency = ([ ]);

    if (!stringp(type)) raise_error("Bad argument 1 to currency->query_currency");
    if (member(type, VALID_CURRENCIES) == -1) return -1;

    if (!__Currency[type]) return 0;
    else return __Currency[type];
}

int add_currency (string type, int amount) {
    int result = 0;

    if (!mappingp(__Currency)) __Currency = ([ ]);

    if (!stringp(type) || member(type, VALID_CURRENCIES) == -1) raise_error("Bad argument 1 to currency->add_currency");
    if (!amount || !intp(amount)) raise_error("Bad argument 2 to currency->add_currency");

    if (!__Currency[type]) {
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