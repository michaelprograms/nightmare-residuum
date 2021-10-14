inherit STD_CONTAINER;
inherit M_EXIT;

void create () {
    container::create();
}

int is_room () { return 1; }

object *query_living_contents () {
    return filter_array(all_inventory(this_object()), (: $1->is_living() :));
}
object *query_nonliving_contents () {
    return filter_array(all_inventory(this_object()), (: !$1->is_living() :));
}