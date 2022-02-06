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
        if (file_size(input) == -1) {
            input = this_user()->query_shell()->query_variable("cwd") + "/" + input;
        }
        write("Going...\n");
        this_character()->handle_go(input, "teleport", "away");
        this_character()->describe_environment();
    }
}