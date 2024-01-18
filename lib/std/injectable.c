inherit STD_ITEM;

nosave private string __Type = "";
nosave private int __Strength = 0;

int is_injectable () { return 1; }

void set_type (string t) {
    __Type = t;
}
string query_type () {
    return __Type;
}
void set_strength (int s) {
    __Strength = s;
}
int query_strength () {
    return __Strength;
}

void handle_inject (object character) {
    message("action", "You inject " + query_name() + " of " + __Type + ".", character);
    message("action", character->query_cap_name() + " injects " + query_name() + " of " + __Type + ".", environment(character), character);

    // @TODO inject logic

    character->set_disable(1);
    handle_remove();
}

mixed direct_inject_obj (mixed args...) {
    return environment() == previous_object();
}