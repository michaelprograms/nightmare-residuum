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

    set_name("Vendor");
    set_short("vendor");
    set_long("A vendor.");
    set_level(1);
    set_species("human");
    set_gender(element_of(({ "male", "female" })));

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

// mixed direct_buy_str_from_liv (string str) { }