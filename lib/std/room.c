inherit STD_OBJECT;

inherit M_CLEAN;
inherit M_CONTAINER;
inherit M_EXIT;
inherit M_LOOK;
inherit M_PROPERTY;
inherit M_RESET;

void create () {
    object::create();
    reset::create();
    reset();

    if (this_object()) {
        D_ASTRONOMY->handle_room_create (this_object());
    }
}

void reset () {
    reset::reset();
}

int is_room () { return 1; }

int clean_up (mixed *args...) {
    foreach (object ob in deep_inventory()) {
        if (ob->query_user() && userp(ob->query_user())) {
            return ::clean_later();
        }
    }
    return ::clean_up();
}

int handle_receive (object ob) {
    if (ob) {
        if (ob->is_living()) {
            object *obs = query_living_contents() + query_item_contents() - ({ ob });
            foreach (object o in obs) {
                // call out to delay fn til after move
                call_out_walltime(function (object ob, object o) {
                    if (objectp(ob) && objectp(o)) {
                        return o->handle_receive_living_in_env(ob);
                    }
                }, 0, ob, o);
            }
        } else if (ob->is_item()) {
            object *obs = query_living_contents() + query_item_contents() - ({ ob });
            foreach (object o in obs) {
                // call out to delay fn til after move
                call_out_walltime(function (object ob, object o) {
                    if (objectp(ob) && objectp(o)) {
                        return o->handle_receive_item_in_env(ob);
                    }
                }, 0, ob, o);
            }
        }
    }
    return ::handle_receive(ob);
}

int handle_release (object ob) {
    if (ob) {
        if (ob->is_living()) {
            object *obs = query_living_contents() + query_item_contents() - ({ ob });
            foreach (object o in obs) {
                o->handle_release_living_in_env(ob);
            }
        } else if (ob->is_item()) {
            object *obs = query_living_contents() + query_item_contents() - ({ ob });
            foreach (object o in obs) {
                o->handle_release_item_in_env(ob);
            }
        }
    }
    return ::handle_release(ob);
}