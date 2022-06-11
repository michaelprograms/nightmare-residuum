#include <verb.h>

inherit STD_VERB;

void create () {
    verb::create();
    add_rules(({ "", "OBJ", "OBJ to OBJ", }));
    set_requirements(REQUIREMENT_BUSY | REQUIREMENT_DISABLE);
}

/* ----- OBJ to OBJ ----- */

mixed can_sell_obj_to_obj (mixed args...) {
    return 1;
}
void do_sell_obj_to_obj (mixed args...) {
    object po = previous_object();
    object vendor;
    object ob;

    if (sizeof(args) > 1) {
        ob = args[0];
        vendor = args[1];
        vendor->handle_sell(ob, po);
    } else {
        message("action", "Buy what from which vendor?\n", po);
        return;
    }
}

/* ----- OBJ ----- */

mixed can_sell_obj (mixed args...) {
    return 1;
}
void do_sell_obj (mixed args...) {
    object po = previous_object();
    object *vendors;

    if (environment(po) && sizeof(args)) {
        vendors = filter(environment(po)->query_living_contents(), (: $1->is_vendor() :));
        if (sizeof(vendors) == 1) {
            do_sell_obj_to_obj(args[0], vendors[0]); // , args[0], vendors[0]->query_name());
        } else {
            message("action", "Sell what to which vendor?\n", po);
        }
    }
}

/* ----- no input ----- */

mixed can_sell (mixed args...) {
    return 1;
}
void do_sell (mixed args...) {
    message("action", "Sell what to which vendor?\n", previous_object());
}