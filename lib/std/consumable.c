inherit STD_ITEM;

nosave private int __Strength = 0;

int is_consumable () { return 1; }
int is_drink () { return 0; }
int is_food () { return 0; }

void set_strength (int s) {
    __Strength = s;
}
int query_strength () {
    return __Strength;
}

void handle_consume (object character) {
    string verb;

    if (is_drink()) {
        verb = "drink";
    } else if (is_food()) {
        verb = "eat";
    }

    message("action", "You " + verb + " " + query_name() + ".", character);
    message("action", character->query_cap_name() + " " + verb + "s " + query_name() + ".", environment(character), character);

    character->add_hp(__Strength);
    character->add_sp(__Strength);
    character->add_mp(__Strength);
    character->set_disable(1);
    handle_remove();
}