inherit STD_NPC;

nosave private object __VendorInventory;

int is_vendor () { return 1; }

object query_vendor_inventory () { return __VendorInventory; }

void set_max_items (int n) {
    if (__VendorInventory) {
        return __VendorInventory->set_max_items(n);
    }
}

void create () {
    ::create();
    if (clonep()) {
        // create vendor inventor
        __VendorInventory = clone_object("/std/vendor_inventory.c");
    }
}

int handle_remove () {
    if (__VendorInventory) {
        __VendorInventory->handle_remove();
    }
    return ::handle_remove();
}

mixed direct_list_obj (mixed args...) {
    object po = previous_object();
    object ob;
    if (sizeof(args)) {
        ob = args[0];
        return environment(ob) == environment(po);
    } else return 0;
}

// mixed direct_buy_str_from_liv (string str) { }