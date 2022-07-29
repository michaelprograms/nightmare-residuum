
nosave private mapping __Listens;
nosave private mapping __Smells;

void create () {
    __Listens = ([ ]);
    __Smells = ([ ]);
}

mixed query_listen (string item) {
    if (!stringp(item)) error("Bad argument 1 to senses->query_listen");
    return __Listens[item];
}
void set_listen (string item, mixed value) {
    if (!stringp(item)) error("Bad argument 1 to senses->set_listen");
    if (!stringp(value) && !functionp(value)) error("Bad argument 2 to senses->set_listen");
    __Listens[item] = value;
}

mixed query_smell (string item) {
    if (!stringp(item)) error("Bad argument 1 to senses->query_smell");
    return __Smells[item];
}
void set_smell (string item, string value) {
    if (!stringp(item)) error("Bad argument 1 to senses->set_listen");
    if (!stringp(value) && !functionp(value)) error("Bad argument 2 to senses->set_listen");
    __Smells[item] = value;
}