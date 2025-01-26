inherit STD_ITEM;

nosave private int __Strength = 0;

int is_injectable () {
    return 1;
}

void set_strength (int s) {
    __Strength = s;
}
int query_strength () {
    return __Strength;
}

/**
 * Inject this item into the target.
 *
 * @param {STD_LIVING} target the target of the injection
 */
void handle_inject (object target) {
    target->add_injection(query_type(), __Strength);
    // @TODO add empty container
    handle_remove();
}

/* ----- parser applies ----- */

mixed direct_inject_obj (mixed args...) {
    return environment() == previous_object();
}
mixed direct_inject_obj_into_liv (mixed args...) {
    return environment() == previous_object();
}