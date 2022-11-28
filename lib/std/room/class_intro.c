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
        message("action", "You are held back by some " + pluralize(__Class) + ".", ob);
        return 0;
    }
    return 1;
}

void create () {
    ::create();

    parse_init();
    parse_add_rule("become", "STR");
}

mixed can_become_str (mixed args...) {
    return environment(previous_object()) == this_object();
}
void do_become_str (mixed args...) {
    object po = previous_object();
    string str;

    if (sizeof(args) > 0) {
        str = args[0];
    } else {
        return;
    }

    if (str != query_class()) {
        message("action", "You can only become a " + query_class() + " here.", po);
        return;
    }
    if (po->query_class() != "adventurer") {
        message("action", "You already have a class.", po);
        return;
    }

    message("action", "You become a " + str + ".", po);
    po->set_class(str);
}