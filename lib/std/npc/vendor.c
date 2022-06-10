inherit STD_NPC;

nosave private object __VendorInventory;
nosave private string __VendorCurrency;
nosave private string __VendorType;

int is_vendor () { return 1; }

object query_vendor_inventory () { return __VendorInventory; }

void set_max_items (int n) {
    if (__VendorInventory) {
        return __VendorInventory->set_max_items(n);
    }
}

string query_vendor_currency () {
    return __VendorCurrency;
}
void set_vendor_currency (string c) {
    if (!stringp(c)) error("Bad argument 1 to vendor->set_vendor_currency");
    __VendorCurrency = c;
}

string query_vendor_type () {
    return __VendorType;
}
void set_vendor_type (string t) {
    if (!stringp(t)) error("Bad argument 1 to vendor->set_vendor_type");
    __VendorType = t;
}

int handle_remove () {
    if (__VendorInventory) {
        __VendorInventory->handle_remove();
    }
    return ::handle_remove();
}

/* ----- ----- */

void create () {
    ::create();
    if (clonep()) {
        // create vendor inventor
        __VendorInventory = clone_object("/std/vendor_inventory.c");
    }
}

/* ----- parser applies ----- */

mixed direct_list_obj (mixed args...) {
    object po = previous_object();
    object ob;
    if (sizeof(args)) {
        ob = args[0];
        return environment(ob) == environment(po);
    } else return 0;
}

// mixed direct_buy_str_from_liv (string str) { }