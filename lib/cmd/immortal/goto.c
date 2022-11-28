void command (string input, mapping flags) {
    object ob;

    if (!input) {
        message("action", "Goto where or who?", this_character());
        return;
    }

    if (ob = find_character(input)) {
        message("action", "Going to "+input+"...", this_character());
        this_character()->handle_go(environment(ob), "teleport", "away");
        this_character()->describe_environment();
        return;
    } else {
        if (input[<2..] != ".c") {
            input += ".c";
        }
        if (file_size(input) == -1) {
            input = this_user()->query_shell()->query_variable("cwd") + "/" + input;
        }
        if (file_size(input) == -1) {
            message("action", "goto: " + input + " not found.", this_character());
            return;
        }
        message("action", "Going...", this_character());
        this_character()->handle_go(input, "teleport", "away");
        this_character()->describe_environment();
    }
}