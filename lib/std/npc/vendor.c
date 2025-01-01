inherit STD_NPC;

/** @type {/std/vendor_inventory} */
nosave private object __VendorInventory;
nosave private string __VendorCurrency;
nosave private string *__VendorTypes;
nosave private int __VendorMaxItems = 0;

int is_vendor () {
    return 1;
}

object query_vendor_inventory () {
    return __VendorInventory;
}

void set_vendor_max_items (int n) {
    __VendorMaxItems = n;
}
int query_vendor_max_items () {
    return __VendorMaxItems;
}

string query_vendor_currency () {
    return __VendorCurrency;
}
void set_vendor_currency (string c) {
    if (!stringp(c)) {
        error("Bad argument 1 to vendor->set_vendor_currency");
    }
    __VendorCurrency = c;
}

string *query_vendor_types () {
    return __VendorTypes;
}
void set_vendor_types (string *types) {
    if (!arrayp(types) || !sizeof(types)) {
        error("Bad argument 1 to vendor->set_vendor_types");
    }
    __VendorTypes = types;
}

int handle_remove () {
    if (__VendorInventory) {
        __VendorInventory->clear_inventory();
        __VendorInventory->handle_remove();
    }
    return ::handle_remove();
}

/* ----- list ----- */

/**
 * Lists out the vendor inventory's items.
 *
 * @param str the text to filter inventory items by
 * @param {STD_LIVING} po the previous object requesting the list
 */
void handle_list (string str, object po) {
    object *items = ({ });

    if (str) {
        items = filter(__VendorInventory->query_item_contents(), (: $1 && (strsrch($1->query_name(), $(str)) > -1 || $1->query_type() == $(str)) :));
    } else {
        items = __VendorInventory->query_item_contents();
    }

    if (!sizeof(items)) {
        if (str) {
            handle_command("say I don't have any '" + str + "' for sale.");
        } else {
            handle_command("say I don't have any items for sale right now, " + po->query_cap_name() + ".");
        }
        return;
    }

    if (str) {
        handle_command("say I have the following '" + str + "' items, " + po->query_cap_name() + ".");
    } else {
        handle_command("say I have the following items, " + po->query_cap_name() + ".");
    }
    foreach (object ob in items) {
        message("action", sprintf("  %-30s%s %s", ob->query_short(), format_integer(ob->query_value()), query_vendor_currency()), po);
    }

}

/* ----- buy ----- */

/**
 * Buy an item from the vendor inventory.
 *
 * @param str the text to filter inventory items by
 * @param {STD_LIVING} po the previous object requesting the list
 */
void handle_buy (string str, object po) {
    /** @type {STD_ITEM} */
    object item;
    int value;

    if (!(item = present(str, __VendorInventory))) {
        handle_command("say I don't have any '" + str + "' for sale.");
        return;
    }

    value = item->query_value();
    if (value > po->query_currency(__VendorCurrency)) {
        handle_command("say You can't afford " + item->query_short() + ".");
        return;
    }

    handle_command("say Here's your " + item->query_short() + ", " + po->query_cap_name() + "!");
    message("action", "You buy " + item->query_short() + " for " + value + " " + __VendorCurrency + ".", po);
    message("action", po->query_cap_name() + " buys " + item->query_short() + ".", environment(po), po);

    po->add_currency(__VendorCurrency, -value);
    if (!item->handle_move(po)) {
        message("action", "You cannot hold " + item->query_short() + " and it falls from your grasp.", po);
        message("action", po->query_cap_name() + " cannot hold " + item->query_short() + " and it falls from " + possessive(po) + " grasp.", environment(po), po);
        item->handle_move(environment(po));
    }
}

/* ----- sell ----- */

/**
 * Sell an item to the vendor.
 *
 * @param {STD_ITEM} item the object being sold to the vendor
 * @param {STD_LIVING} po the previous object selling the item
 */
void handle_sell (object item, object po) {
    int value;

    if (!item) {
        handle_command("say You don't have an item to sell.");
        return;
    }
    // @TODO: this doesn't work because item->query_type doesn't exist, and __VendorTypes is an array of inheritables
    if (!sizeof(__VendorTypes) || member_array(__VendorTypes, item->query_type()) == -1) {
        handle_command("say I don't buy " + item->query_type() + " items.");
        return;
    }
    if (__VendorInventory->query_item_contents() >= __VendorMaxItems) {
        handle_command("say My shop is full, I can't buy any more items.");
        return;
    }
    value = item->query_value() * 50 / 100;
    po->add_currency(__VendorCurrency, value);
    message("action", "You sell " + item->query_short() + " for " + value + " " + __VendorCurrency + ".", po);
    message("action", po->query_cap_name() + " sells " + item->query_short() + ".", environment(po), po);
}

/* ----- object applies ----- */

void create () {
    ::create();
    if (clonep()) {
        __VendorInventory = new("/std/vendor_inventory.c");
        __VendorInventory->handle_move(this_object());
    }
}

/* ----- parser applies ----- */

mixed direct_list_from_obj (mixed args...) {
    object po = previous_object();
    object ob;
    if (sizeof(args)) {
        ob = args[0];
        return environment(ob) == environment(po);
    } else {
        return 0;
    }
}
mixed direct_list_str_from_obj (mixed args...) {
    object po = previous_object();
    object ob;
    if (sizeof(args) > 1) {
        ob = args[1];
        return environment(ob) == environment(po);
    } else {
        return 0;
    }
}
mixed direct_buy_str_from_obj (mixed args...) {
    object po = previous_object();
    object ob;
    if (sizeof(args) > 1) {
        ob = args[1];
        return environment(ob) == environment(po);
    } else {
        return 0;
    }
}