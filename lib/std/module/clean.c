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
    object env, *inv;
    int i;

    if (env = environment(this_object())) {
        i = sizeof(inv = all_inventory(this_object()));
        while (i--) {
            if (inv[i]) inv[i]->eventMove(env); // @TODO should clean_up instead?
        }
    }
    destruct(this_object());
    return !(this_object());
}
int handle_remove () {
    return internal_remove();
}

int clean_up () {
    if (origin() == "driver" || environment() || __NoClean) {
        return clean_never();
    }
    if (!function_exists("handle_remove", this_object())) {
        debug_message(identify(this_object())+" tried to clean_up but no handle_remove");
    }
    return this_object()->handle_remove();
}