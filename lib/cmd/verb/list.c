#include <verb.h>

inherit STD_VERB;

void create () {
    verb::create();
    add_rules(({ "", "STR from OBJ", "from OBJ", "STR", }));
    set_requirements(REQUIREMENT_BUSY | REQUIREMENT_DISABLE);
}

/* ----- STR from OBJ ----- */

mixed can_list_str_from_obj (mixed args...) {
    return 1;
}
void do_list_str_from_obj (mixed args...) {
    object po = previous_object();
    string str;
    object vendor;

    if (sizeof(args) > 1) {
        str = args[0];
        vendor = args[1];
        vendor->handle_list(str, po);
    } else {
        message("action", "List from which vendor?\n", po);
        return;
    }
}

/* ----- from OBJ ----- */

mixed can_list_from_obj (mixed args...) {
    return 1;
}
void do_list_from_obj (mixed args...) {
    object po = previous_object();
    object vendor;

    if (sizeof(args)) {
        vendor = args[0];
        vendor->handle_list(0, po);
    } else {
        message("action", "List from which vendor?\n", po);
        return;
    }
}

/* ----- STR ----- */

mixed can_list_str (mixed args...) {
    return 1;
}
void do_list_str (mixed args...) {
    object po = previous_object();
    object *vendors;
    string str;

    if (sizeof(args) && environment(po)) {
        str = args[0];
        vendors = filter(environment(po)->query_living_contents(), (: $1->is_vendor() :));
        if (sizeof(vendors) == 1) {
            vendors[0]->handle_list(str, po);
        } else {
            message("action", "List from which vendor?\n", po);
        }
    }
}

/* ----- no input ----- */

mixed can_list (mixed args...) {
    return 1;
}
void do_list (mixed args...) {
    object po = previous_object();
    object *vendors;

    if (environment(po)) {
        vendors = filter(environment(po)->query_living_contents(), (: $1->is_vendor() :));
        if (sizeof(vendors) == 1) {
            vendors[0]->handle_list(0, po);
        } else {
            message("action", "List from which vendor?\n", po);
        }
    }
}