#include <verb.h>

inherit STD_VERB;

void create () {
    verb::create();
    add_rules(({ "", "STR from OBJ", "STR", }));
    set_requirements(REQUIREMENT_BUSY | REQUIREMENT_DISABLE);
    set_syntax("buy [item]");
    set_help_text("Your character will attempt to buy the provided item.");
    set_help_similar(({ "list", "sell", }));
}

/* ----- STR from OBJ ----- */

mixed can_buy_str_from_obj (mixed args...) {
    return 1;
}
void do_buy_str_from_obj (mixed args...) {
    object po = previous_object();
    object vendor;
    string str;

    if (sizeof(args) > 1) {
        str = args[0];
        vendor = args[1];
        vendor->handle_buy(str, po);
    } else {
        message("action", "Buy what from which vendor?", po);
        return;
    }
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
            do_buy_str_from_obj(args[0], vendors[0]);
        } else {
            message("action", "Buy what from which vendor?", po);
        }
    }
}

/* ----- no input ----- */

mixed can_buy (mixed args...) {
    return 1;
}
void do_buy (mixed args...) {
    message("action", "Buy what from which vendor?", previous_object());
}