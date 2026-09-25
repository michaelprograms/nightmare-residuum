inherit STD_CONSUMABLE;

int is_drink() {
    return 1;
}

string query_use_verb() {
    return "drink";
}

mixed direct_drink_obj(mixed _args...) {
    return environment() == previous_object();
}


void create() {
    ::create();
    set_type("drink");
}
