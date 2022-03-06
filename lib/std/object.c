inherit "/std/module/description.c";
inherit "/std/module/id.c";

// -----------------------------------------------------------------------------

nosave private object __LastEnv = 0;

// -----------------------------------------------------------------------------

int handle_move (mixed dest) {
    object destOb;
    int x;

    if (!this_object()) return 0;
    if (environment()) {
        x = environment()->can_release(this_object());
        if (!x /* && !archp(this_object()) */) return 0; // @TODO archp? assistp?
    }
    if (stringp(dest)) {
        if (!(destOb = find_object(dest))) {
            if (catch(load_object(dest)) ) {
                return 0;
            }
            destOb = find_object(dest);
        }
    } else {
        destOb = dest;
    }
    if (!destOb || destOb == this_object()) return 0;
    // if (living(this_object()) && living(destOb)) return 0; // @TODO
    if (!destOb->can_receive(this_object())) return 0;
    if (__LastEnv = environment()) {
        environment()->handle_release(this_object());
    }
    move_object(destOb);
    environment()->handle_receive(this_object());
    return (__LastEnv != environment());
}

// --- APPLIES -----------------------------------------------------------------

void create () {
    id::create();
}

// void heart_beat () {
//     /*
//     If an object has called set_heart_beat(), this function will be called
//     periodically in that object.
//     */
// }

int id (string id) {
    /*
    The  present(3)  efunction calls id() to determine if a given object is
    named by a given string.  id() should return 1 if the object wishes  to
    be known by the name in the string an Id; it should return 0 otherwise.
    */
    return member_array(id, query_id()) > -1;
}

// @TODO
// int move_or_destruct (object parent) {
    // Interactive should handle this.
    /*
    If an object is to be destructed, prior to its destruction this apply is called on its contents. 'parent' is the object that will be destructed. If the target object of this apply does not move itself out of the object being destructed, it will be destructed as well.
    */
// }

void reset () {
}

// @TODO
// void virtual_start () {
//     /*
//     After virtual object is created, driver will call this function to trigger
//     addtional initialization.
//     */
// }

// --- PARSER ------------------------------------------------------------------

int is_living () {
    return 0;
}
int inventory_visible () {
    return 0;
}
int inventory_accessible () {
    return 0;
}