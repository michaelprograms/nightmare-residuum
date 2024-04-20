inherit STD_NPC;

nosave private object __VendorInventory;
nosave private string __VendorCurrency;
nosave private string *__VendorTypes;

int is_vendor () {
    return 1;
}

object query_vendor_inventory () {
    return __VendorInventory;
}

void set_max_items (int n) {
    if (__VendorInventory) {
        return __VendorInventory->set_max_items(n);
    }
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
        __VendorInventory->handle_remove();
    }
    return ::handle_remove();
}

/* ----- list ----- */

void handle_list (string str, object po) {
    object *items = ({ });

    if (str) {
        items = filter(__VendorInventory->query_item_contents(), (: $1 && (strsrch($1->query_name(), $(str)) > -1 || $1->query_type() == $(str)) :));
    } else {
        items = __VendorInventory->query_item_contents();
    }

    if (!sizeof(items)) {
        if (str) {
            do_command("say I don't have any '" + str + "' for sale.");
        } else {
            do_command("say I don't have any items for sale right now, " + po->query_cap_name() + ".");
        }
        return;
    }

    if (str) {
        do_command("say I have the following '" + str + "' items, " + po->query_cap_name() + ".");
    } else {
        do_command("say I have the following items, " + po->query_cap_name() + ".");
    }
    foreach (object ob in items) {
        message("action", sprintf("  %-30s%s %s", ob->query_short(), format_integer(ob->query_value()), query_vendor_currency()), po);
    }

}

/* ----- buy ----- */

void handle_buy (string str, object po) {
    object item;
    int value;

    if (!(item = present(str, __VendorInventory))) {
        do_command("say I don't have any '" + str + "' for sale.");
        return;
    }

    value = item->query_value();

    if (value > po->query_currency(__VendorCurrency)) {
        do_command("say You can't afford " + item->query_short() + ".");
        return;
    }

    do_command("say Here's your " + item->query_short() + ", " + po->query_cap_name() + "!");
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

void handle_sell (object item, object po) {
    int value;

    if (!item) {
        do_command("say You don't have an item to sell.");
        return;
    }

    value = item->query_value() * 50 / 100;

    if (!item->handle_move(__VendorInventory)) {
        do_command("say My shop is full, I can't buy any more items.");
        return;
    }
    po->add_currency(__VendorCurrency, value);
    message("action", "You sell " + item->query_short() + " for " + value + " " + __VendorCurrency + ".", po);
    message("action", po->query_cap_name() + " sells " + item->query_short() + ".", environment(po), po);
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