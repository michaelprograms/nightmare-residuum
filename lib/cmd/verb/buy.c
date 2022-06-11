#include <verb.h>

inherit STD_VERB;

void create () {
    verb::create();
    add_rules(({ "", "STR from OBJ", "STR", }));
    set_requirements(REQUIREMENT_BUSY | REQUIREMENT_DISABLE);
}

/* ----- no input ----- */

mixed can_buy () {
    return 1;
}
void do_buy (mixed args...) {
    message("action", "Buy which item?\n", previous_object());
}

/* ----- STR from OBJ ----- */

mixed can_buy_str_from_obj (mixed args...) {
    return 1;
}
void do_buy_str_from_obj (mixed args...) {
    object po = previous_object();
    object vendor;
    string strItem;

    if (sizeof(args) < 2) {
        message("action", "Buy what from which vendor?\n", po);
        return;
    }
    // when called from do_buy_str, args[0] and args[2] won't contain `from OBJ`
    strItem = explode(args[0], " from")[0];
    vendor = args[1];

    vendor->handle_buy(strItem, po);
}

/* ----- STR ----- */

mixed can_buy_str (mixed args...) {
    return 1;
}
void do_buy_str (mixed args...) {
    object po = previous_object();
    object *vendors;

    if (environment(po) && sizeof(args)) {
        vendors = filter(environment(po)->query_living_contents(), (: $1->is_vendor() :));
        if (sizeof(vendors) == 1) {
            do_buy_str_from_obj(args[0], vendors[0], args[0], vendors[0]->query_name());
        } else {
            message("action", "Buy what from which vendor?\n", po);
        }
    }
}