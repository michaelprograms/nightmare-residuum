
nosave mapping __Listens;
nosave mapping __Smells;

private void initialize_senses () {
    if (!mapp(__Listens)) {
        __Listens = ([ ]);
    }
    if (!mapp(__Smells)) {
        __Smells = ([ ]);
    }
}

mixed query_listen (string item) {
    initialize_senses();
    return __Listens[item];
}
void set_listen (string item, mixed value) {
    initialize_senses();
    __Listens[item] = value;
}
mapping query_listens () {
    initialize_senses();
    return __Listens;
}

mixed query_smell (string item) {
    initialize_senses();
    return __Smells[item];
}
void set_smell (string item, mixed value) {
    initialize_senses();
    __Smells[item] = value;
}
mapping query_smells () {
    initialize_senses();
    return __Smells;
}