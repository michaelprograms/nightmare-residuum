inherit STD_OBJECT;
inherit M_CLEAN;

int can_receive (object ob) {
    return member_array(ob, all_inventory(this_object())) == -1;
}
int can_release (object ob) {
    return member_array(ob, all_inventory(this_object())) > -1;
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