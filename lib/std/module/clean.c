nosave private int __NoClean = 0;

/**
 * Query if the object has been flagged as 'no clean'.
 *
 * @returns 0 or 1
 */
int query_no_clean () {
    return __NoClean;
}
/**
 * Set the object's 'no clean' flag.
 *
 * @param value 0 or 1
 */
void set_no_clean (int value) {
    __NoClean = value;
}

/**
 * Helper function to return the clean never state.
 *
 * @returns 0
 */
nomask int clean_never () {
    return 0;
}
/**
 * Helper function to return the clean later state.
 *
 * @returns 1
 */
nomask int clean_later () {
    return 1;
}

/**
 * This private function is used to actually remove an object.
 *
 * @returns 0 or 1 for success
 */
protected int internal_remove () {
    object env;
    if (env = environment()) {
        map(all_inventory(), (: $1->handle_move($(env)) :));
    }
    destruct();
    return !this_object();
}

/**
 * This function is used to remove an object from the game.
 *
 * @returns 0 or 1 for success
 */
int handle_remove () {
    return internal_remove();
}

/**
 * This apply is periodically called upon inactive objects.
 *
 * Be wary of calling functions in other objects in clean_up, which may cause
 * that object's clean_up timer to be reset.
 *
 * @param args an integer argument of number of inherited blueprints (do not trust this value)
 */
int clean_up (mixed args...) {
    // Objects never lose environment, let environment handle clean_up
    if (environment()) {
        return clean_never();
    }
    // Objects (mainly rooms or containers) can be flagged to never clean
    if (__NoClean) {
        return clean_never();
    }
    return handle_remove();
}