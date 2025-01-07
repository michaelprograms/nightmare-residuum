void handle_expire () {
    // @lpc-ignore
    message("action", "The " + this_object()->query_short() + " flakes into dust.", environment());
    remove_call_out();
    // @lpc-ignore
    this_object()->handle_remove();
}

void handle_received (object env) {
    call_out((: handle_expire :), 120);
}