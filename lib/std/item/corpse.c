inherit STD_STORAGE;

void create () {
    storage::create();
    set_name("corpse");
    set_id(({"corpse"}));
    set_short("a corpse");
    set_long("A corpse.");
}

void handle_expire () {
    if (environment()) {
        message("system", "The corpse dissolves into dust.\n", environment());
    }
    remove_call_out();
    handle_remove();
}

void handle_received (object env) {
    ::handle_received(env);
    call_out((: handle_expire :), 60 * 5);
}