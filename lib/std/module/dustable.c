void handle_expire () {
    if (environment()) {
        message("system", "The " + this_object()->query_short() + " flakes into dust.\n", environment());
    }
    remove_call_out();
    this_object()->handle_remove();
}

void handle_received (object env) {
    call_out((: handle_expire :), 120);
}