inherit STD_ROOM;

nosave private string __Class;

string query_class () {
    return __Class;
}
void set_class (string c) {
    __Class = c;
}

int handle_pre_exit (object ob, string dir) {
    if (!ob || (ob->query_class() != __Class && !ob->query_immortal())) {
        message("action", "You are held back by some " + pluralize(__Class) + ".\n", ob);
        return 0;
    }
    return 1;
}