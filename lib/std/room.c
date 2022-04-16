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
}

void reset () {
    reset::reset();
}

int is_room () { return 1; }

int clean_up (int inherited) {
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
            foreach (object o in query_living_contents() + query_item_contents() - ({ ob })) {
                // call out to delay fn til after move
                call_out_walltime(function (object ob, object o) {
                    if (objectp(ob) && objectp(o)) {
                        return o->handle_receive_living_in_env(ob);
                    }
                }, 0, ob, o);
            }
        } else if (ob->is_item()) {
            foreach (object o in query_living_contents() + query_item_contents() - ({ ob })) {
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
            foreach (object o in query_living_contents() + query_item_contents() - ({ ob })) {
                o->handle_release_living_in_env(ob);
            }
        } else if (ob->is_item()) {
            foreach (object o in query_living_contents() + query_item_contents() - ({ ob })) {
                o->handle_release_item_in_env(ob);
            }
        }
    }
    return ::handle_release(ob);
}

int direct_verb_liv (mixed args...) {
    return 0;
}