int can_receive (object ob) {
    return member(ob, all_inventory()) == -1;
}
int can_release (object ob) {
    return member(ob, all_inventory()) > -1;
}

int handle_receive (object ob) {
    return !!ob;
}
int handle_release (object ob) {
    return !!ob;
}

int inventory_visible () {
    return 1;
}
int inventory_accessible () {
    return 1;
}

object *query_living_contents () {
    return filter(all_inventory(), (: $1->is_living() :));
}
object *query_item_contents () {
    return filter(all_inventory(), (: !$1->is_living() :));
}