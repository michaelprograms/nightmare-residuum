
nosave private mapping __Listens;
nosave private mapping __Smells;

void create () {
    __Listens = ([ ]);
    __Smells = ([ ]);
}

mixed query_listen (string item) {
    return __Listens[item];
}
void set_listen (string item, mixed value) {
    __Listens[item] = value;
}

mixed query_smell (string item) {
    return __Smells[item];
}
void set_smell (string item, mixed value) {
    __Smells[item] = value;
}