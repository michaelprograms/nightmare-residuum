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

/**
 * Handle a living object consume this item.
 *
 * @param {STD_LIVING} living who is consuming this consumable
 */
void handle_consume (object living) {
    string verb = this_object()->query_use_verb();

    message("action", "You " + verb + " " + query_name() + ".", living);
    message("action", living->query_cap_name() + " " + verb + "s " + query_name() + ".", environment(living), living);

    living->add_hp(__Strength);
    living->add_sp(__Strength);
    living->add_mp(__Strength);
    living->set_disable(1);
    // @TODO add empty container
    handle_remove();
}