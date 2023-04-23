inherit STD_ITEM;

nosave private int __Strength = 0;

int is_drink () { return 1; }

void set_strength (int s) {
    __Strength = s;
}
int query_strength () {
    return __Strength;
}

mixed direct_drink_obj (mixed args...) {
    return environment() == previous_object();
}

void handle_drink (object character) {
    message("action", "You drink " + query_name() + ".", character);
    message("action", character->query_cap_name() + " drinks " + query_name() + ".", environment(character), character);
    character->add_hp(__Strength);
    character->add_sp(__Strength);
    character->add_mp(__Strength);
    character->selazygitt_disable(1);
    handle_remove();
}