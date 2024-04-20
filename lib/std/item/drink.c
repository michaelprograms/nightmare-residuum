inherit STD_CONSUMABLE;

int is_drink () {
    return 1;
}

mixed direct_drink_obj (mixed args...) {
    return environment() == previous_object();
}