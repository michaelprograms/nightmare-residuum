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
    if (env = environment(this_object())) {
        map(all_inventory(this_object()), (: $1->handle_move($(env)) :));
    }
    destruct(this_object());
    return !(this_object());
}
int handle_remove () {
    return internal_remove();
}

int clean_up (int inherited) {
    if (origin() == "driver" || environment() || __NoClean) {
        return clean_never();
    }
    return this_object()->handle_remove();
}