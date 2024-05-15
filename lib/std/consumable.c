inherit STD_ITEM;

nosave private int __Strength = 0;

int is_consumable () {
    return 1;
}
int is_drink () {
    return 0;
}
int is_food () {
    return 0;
}

void set_strength (int s) {
    __Strength = s;
}
int query_strength () {
    return __Strength;
}

string query_use_verb () {
    return "consume";
}

void handle_consume (object character) {
    string verb = this_object()->query_use_verb();

    message("action", "You " + verb + " " + query_name() + ".", character);
    message("action", character->query_cap_name() + " " + verb + "s " + query_name() + ".", environment(character), character);

    character->add_hp(__Strength);
    character->add_sp(__Strength);
    character->add_mp(__Strength);
    character->set_disable(1);
    // @TODO add empty container
    handle_remove();
}