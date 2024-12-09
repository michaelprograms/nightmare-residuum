nosave private int __NoClean = 0;

int query_no_clean () {
    return __NoClean;
}
void set_no_clean (int value) {
    __NoClean = value;
}

nomask int clean_never () {
    return 0;
}
nomask int clean_later () {
    return 1;
}

protected int internal_remove () {
    object env;
    if (env = environment()) {
        map(all_inventory(), (: $1->handle_move($(env)) :));
    }
    destruct();
    return !this_object();
}
int handle_remove () {
    return internal_remove();
}

// Be wary of calling functions in other objects in clean_up, which may cause
// that object's clean_up timer to be reset.
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