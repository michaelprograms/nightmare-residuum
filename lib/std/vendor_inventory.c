inherit M_CLEAN;
inherit M_CONTAINER;
inherit M_PARENT;
inherit M_RESET;

nosave private int __MaxItems = 0;

int query_max_items () {
    return __MaxItems;
}
void set_max_items (int n) {
    __MaxItems = n;
}

int handle_receive (object ob) {
    if (!ob->is_item()) {
        return 0;
    } else if (__MaxItems > 0 && sizeof(all_inventory()) >= __MaxItems) {
        return 0;
    }
    return ::handle_receive(ob);
}

void create () {
    parent::create();
    reset::create();
}

int clean_up (mixed *args...) {
    return clean::clean_up(args);
}