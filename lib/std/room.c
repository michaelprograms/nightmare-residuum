inherit STD_CONTAINER;
inherit M_PROPERTY;
inherit M_RESET;
inherit "/std/module/exit.c";
inherit "/std/module/look.c";

void create () {
    container::create();
    reset::create();
    reset();
}

void reset () {
    reset::reset();
}

int is_room () { return 1; }

object *query_living_contents () {
    return filter_array(all_inventory(this_object()), (: $1->is_living() :));
}
object *query_item_contents () {
    return filter_array(all_inventory(this_object()), (: !$1->is_living() :));
}

int clean_up (int inherited) {
    foreach (object ob in deep_inventory(this_object())) {
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
                call_out_walltime((: $(o)->handle_receive_living_in_env($(ob)) :), 0);
            }
        } else if (ob->is_item()) {
            foreach (object o in query_living_contents() + query_item_contents() - ({ ob })) {
                // call out to delay fn til after move
                call_out_walltime((: $(o)->handle_receive_item_in_env($(ob)) :), 0);
            }
        }
    }
    return ::handle_receive(ob);
}

int handle_release (object ob) {
    if (ob) {
        if (ob->is_living()) {
            foreach (object o in query_living_contents() + query_item_contents() - ({ ob })) {
                o->handle_release_item_in_env(ob);
            }
        } else if (ob->is_item()) {
            foreach (object o in query_living_contents() + query_item_contents() - ({ ob })) {
                o->handle_release_item_in_env(ob);
            }
        }
    }
    return ::handle_release(ob);
}