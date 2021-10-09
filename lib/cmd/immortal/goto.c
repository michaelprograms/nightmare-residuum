void command (string input) {
    object ob;

    if (!input) {
        write("Goto where?\n");
        return;
    }

    if (ob = find_character(input)) {
        write("Going to "+input+"...\n");
        this_character()->handle_go(environment(ob), "teleport", "away");
        this_character()->describe_environment();
        return;
    } else {
        write("Going...\n");
        this_character()->handle_go(input, "teleport", "away");
        this_character()->describe_environment();
    }

    // if (file_size(input) > 0) {
    // } else {
    //     write("File not found?\n");
    // }
}