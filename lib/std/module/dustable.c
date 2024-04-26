void handle_expire () {
    message("action", "The " + this_object()->query_short() + " flakes into dust.", environment());
    remove_call_out();
    this_object()->handle_remove();
}

void handle_received (object env) {
    call_out((: handle_expire :), 120);
}