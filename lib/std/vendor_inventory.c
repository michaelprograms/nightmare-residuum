inherit M_CLEAN;
inherit M_CONTAINER;
inherit M_PARENT;

nosave private int __MaxItems = 0;

int query_max_items () {
    return __MaxItems;
}
void set_max_items (int n) {
    if (!intp(n)) error("Bad argument 1 to vendor_inventory->set_max_items");
    __MaxItems = n;
}

int handle_receive (object ob) {
    if (!ob->is_item()) return 0;
    if (__MaxItems > 0 && sizeof(all_inventory()) >= __MaxItems) return 0;
    return ::handle_receive(ob);
}