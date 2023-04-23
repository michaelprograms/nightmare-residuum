inherit STD_CONSUMABLE;

int is_food () { return 1; }

mixed direct_eat_obj (mixed args...) {
    return (environment() == previous_object());
}