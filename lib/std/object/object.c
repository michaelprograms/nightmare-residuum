inherit "/std/object/description.c";
inherit "/std/object/id.c";
inherit "/std/object/property.c";

#include "/std/object/object.h"

nosave private object __LastEnv = 0;

// -----------------------------------------------------------------------------

int handle_move (mixed dest) {
    object ob;
    int x;
    string str;

    if (!this_object()) return 0;
    if (environment()) {
        x = environment()->can_release(this_object());
        if (!x /* && !archp(this_object()) */) return 0; // @TODO archp? assistp?
    }
    if (stringp(dest)) {
        if (!(ob = find_object(dest))) {
            if (str = catch(call_other(dest, "???")) ) {
                // if (creatorp()) message("error", str, this_object()); // @TODO creatorp
                return 0;
            }
            ob = find_object(dest);
        }
    } else {
        ob = dest;
    }
    if (!ob || ob == this_object()) return 0;
    // if (living(this_object()) && living(ob)) return 0; // @TODO
    if (!(ob->can_receive(this_object()))) return 0;
    if (__LastEnv = environment()) {
        environment()->handle_release(this_object());
    }
    move_object(ob);
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
int move_or_destruct (object parent) {
    debug_message(identify(this_object())+"->move_or_destruct("+identify(parent)+")");
    // Interactive should handle this.
    /*
    If an object is to be destructed, prior to its destruction this apply is called on its contents. 'parent' is the object that will be destructed. If the target object of this apply does not move itself out of the object being destructed, it will be destructed as well.
    */
}

void reset () {
    /*
    After  every  reset interval (whose exact length is determined on a mud
    by mud basis, but averages around every 2 hours), reset() is called  in
    every  object  that  currently  exists.   If  LAZY_RESETS is defined in
    options.h in the driver, reset() will only be called  in  objects  when
    they  are  touched (near players), so unused objects will not be loaded
    from the swap file to reset.  The reset() call is usually used to check
    if  any treasures and/or monsters in the room have been removed so they
    can be replaced.
    */
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