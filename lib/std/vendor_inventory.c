inherit M_CLEAN;
inherit M_CONTAINER;
inherit M_RESET;
inherit M_MOVE;

nosave private int __MaxItems = 0;

int is_vendor_inventory () {
    return 1;
}

int handle_receive (object ob) {
    if (!ob->is_item()) {
        return 0;
    }
    return ::handle_receive(ob);
}

void create () {
    reset::create();
}

int clean_up (mixed args...) {
    return clean::clean_up(args);
}

void clear_inventory () {
    map(all_inventory(), (: $1->handle_remove() :));
}