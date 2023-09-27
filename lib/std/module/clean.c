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

int clean_up (mixed *args...) {
    if (environment() || __NoClean) {
        return clean_never();
    }
    if (this_object()->query_parent()) {
        return clean_later();
    }
    return handle_remove();
}