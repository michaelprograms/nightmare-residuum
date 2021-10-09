inherit STD_ROOM;

void create () {
    ::create();
    set_short("a tank");
    set_long("A tank.");
    set_no_clean(1);
}

void handle_receive_object () {
    if (previous_object()->is_character()) {
        write("handle_receive_object: "+identify(previous_object())+"\n");
    }
}