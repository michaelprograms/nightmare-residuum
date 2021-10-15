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

int clean_up () {
    // @TODO check driver origin
    if (origin() == "driver" || environment() || __NoClean) {
        return clean_never();
    }
    if (sizeof(children(base_name(this_object()))) > 1) {
        return clean_later();
    }
    return this_object()->handle_remove();
}