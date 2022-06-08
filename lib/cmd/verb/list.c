#include <verb.h>

inherit STD_VERB;

void create () {
    verb::create();
    add_rules(({ "", "OBJ", })); // @TODO STR, OBJ STR
    set_requirements(REQUIREMENT_BUSY | REQUIREMENT_DISABLE);
}

mixed can_list_obj (mixed args...) {
    return 1;
}
void do_list_obj (mixed args...) {
    object po = previous_object();
    object vendor, vi;

    if (sizeof(args)) {
        vendor = args[0];
        vi = vendor->query_vendor_inventory();
    } else {
        message("action", "List from which vendor?\n", po);
        return;
    }

    message("action", vendor->query_cap_name() + " has the following items:\n", po);
    foreach (object ob in vi->query_item_contents()) {
        message("action", sprintf("  %-30s%s %s", ob->query_short(), format_integer(ob->query_value()), vendor->query_vendor_currency()) + "\n", po);
    }
}

mixed can_list () {
    return 1;
}
void do_list (mixed args...) {
    object po = previous_object();
    object *vendors;

    if (environment(po)) {
        vendors = filter(environment(po)->query_living_contents(), (: $1->is_vendor() :));
        if (sizeof(vendors) == 1) {
            do_list_obj(vendors[0]);
        } else {
            message("action", "List from which vendor?\n", po);
        }
    }
}