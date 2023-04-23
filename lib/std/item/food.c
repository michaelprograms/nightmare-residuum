inherit STD_ITEM;

nosave private int __Strength = 0;

int is_food () { return 1; }

void set_strength (int s) {
    __Strength = s;
}
int query_strength () {
    return __Strength;
}

mixed direct_eat_obj (mixed args...) {
    return (environment() == previous_object());
}

void handle_eat (object character) {
    message("action", "You eat " + query_name() + ".", character);
    message("action", character->query_cap_name() + " eats " + query_name() + ".", environment(character), character);
    character->add_hp(__Strength);
    character->add_sp(__Strength);
    character->add_mp(__Strength);
    character->set_disable(1);
    handle_remove();
}