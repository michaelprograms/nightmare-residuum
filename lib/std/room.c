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