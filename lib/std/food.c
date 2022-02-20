inherit STD_ITEM;

nosave private int __Strength = 0;

int is_food () { return 1; }

void set_strength (int s) {
    __Strength = s;
}
int query_strength () { return __Strength; }

mixed direct_eat_obj (object ob, string str) {
    return (environment() == previous_object());
}

void handle_eat (object character) {
    message("action", "You eat " + query_name() + ".\n", character);
    message("action", character->query_name() + " eats " + query_name() + ".\n", environment(character), character);
    character->add_hp(__Strength);
    character->add_sp(__Strength);
    character->add_mp(__Strength);
    handle_remove();
}