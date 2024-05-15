inherit STD_CONSUMABLE;

int is_food () {
    return 1;
}

string query_use_verb () {
    return "eat";
}

mixed direct_eat_obj (mixed args...) {
    return (environment() == previous_object());
}