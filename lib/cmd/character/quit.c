void command (string input, mapping flags) {
    if (!this_character()) {
        write("Quitting out...\n");
    } else {
        write("Quitting out of " + this_character()->query_cap_name() + ".\n");
    }
    this_user()->quit_character();
}