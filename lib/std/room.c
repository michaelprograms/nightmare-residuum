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

void handle_receive_living (object living) {
    foreach (object ob in query_living_contents() + query_item_contents() - ({ living })) {
        ob->handle_receive_living(living);
    }
}
void handle_receive_item (object item) {
    foreach (object ob in query_living_contents() + query_item_contents() - ({ item })) {
        ob->handle_receive_item(item);
    }
}

int handle_receive (object ob) {
    if (ob->is_living()) {
        handle_receive_living(ob);
    } else if (ob->is_item()) {
        handle_receive_item(ob);
    }
    return ::handle_receive(ob);
}