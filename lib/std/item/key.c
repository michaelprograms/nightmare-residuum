inherit STD_ITEM;

int is_key () { return 1; }

int direct_unlock_str_with_obj (mixed args...) {
    object key;
    if (sizeof(args) > 1 && objectp(args[1])) {
        key = args[1];
    }
    if (key && key->is_key()) {
        return 1;
    }
    return 0;
}