void command (string input) {
    object ob;

    if (!input) {
        write("Goto where?\n");
        return;
    }

    if (ob = find_character(input)) {
        write("Going to "+input+"...\n");
        this_character()->handle_move(environment(ob));
        return;
    }

    // if (file_size(input) > 0) {
        write("Going...\n");
        this_character()->handle_move(input);
    // } else {
    //     write("File not found?\n");
    // }
}