inherit STD_ROOM;
inherit M_TRAIN;

nosave private string __Class;

string query_class () {
    return __Class;
}
void set_class (string c) {
    __Class = c;
}

int can_receive (object ob) {
    if (__Class && ob->is_character() && ob->query_class() != __Class && !ob->query_immortal()) {
        return 0;
    } else {
        return ::can_receive(ob);
    }
}

void create () {
    ::create();
}