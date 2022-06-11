inherit STD_NPC;

nosave private object __VendorInventory;
nosave private string __VendorCurrency;
nosave private string *__VendorTypes;

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

string *query_vendor_types () {
    return __VendorTypes;
}
void set_vendor_types (string *types) {
    if (!arrayp(types) || !sizeof(types)) error("Bad argument 1 to vendor->set_vendor_types");
    __VendorTypes = types;
}

int handle_remove () {
    if (__VendorInventory) {
        __VendorInventory->handle_remove();
    }
    return ::handle_remove();
}

/* ----- buy ----- */

void handle_buy (string str, object po) {
    object item;
    int value;

    if (!(item = present(str, __VendorInventory))) {
        do_command("say I don't have " + str + " for sale.");
        return;
    }

    // @TODO bargaining adjustment
    value = item->query_value();

    if (value > po->query_currency(__VendorCurrency)) {
        do_command("say You can't afford " + item->query_short() + ".");
        return;
    }

    do_command("say Here's your " + item->query_short() + ", " + po->query_cap_name() + "!");
    message("action", "You buy " + item->query_short() + " for " + value + " " + __VendorCurrency + ".\n", po);
    message("action", po->query_cap_name() + " buys " + item->query_short() + ".\n", environment(po), po);

    po->add_currency(__VendorCurrency, -item->query_value());
    if (!item->handle_move(po)) {
        message("action", "You cannot hold " + item->query_short() + " and it falls from your grasp.\n", po);
        message("action", po->query_cap_name() + " cannot hold " + item->query_short() + " and it falls from " + possessive(po) + " grasp.\n", environment(po), po);
        item->handle_move(environment(po));
    }
}

/* ----- object applies ----- */

void create () {
    ::create();
    if (clonep()) {
        // create vendor inventor
        __VendorInventory = clone_object("/std/vendor_inventory.c");
    }
}

/* ----- parser applies ----- */

mixed direct_list_from_obj (mixed args...) {
    object po = previous_object();
    object ob;
    if (sizeof(args)) {
        ob = args[0];
        return environment(ob) == environment(po);
    } else return 0;
}

mixed direct_buy_str_from_obj (mixed args...) {
    object po = previous_object();
    object ob;
    if (sizeof(args) > 1) {
        ob = args[1];
        return environment(ob) == environment(po);
    } else return 0;
}