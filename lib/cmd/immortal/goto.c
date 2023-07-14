void command (string input, mapping flags) {
    object ob;
    string path;

    if (!input) {
        message("action", "Goto where or who?", this_character());
        return;
    }

    if (
        (ob = find_character(input)) ||
        (regexp(input, "#") && (ob = find_object(input)))
    ) {
        message("action", "Going to "+input+"...", this_character());
        this_character()->handle_go(environment(ob), "teleport", "away");
        this_character()->describe_environment();
        return;
    } else {
        path = input;
        if (path[<2..] != ".c") {
            path += ".c";
        }
        if (file_size(path) == -1) {
            path = this_user()->query_shell()->query_variable("cwd") + "/" + path;
        }
        if (file_size(path) == -1) {
            message("action", "goto: " + input + " not found.", this_character());
            return;
        }
        message("action", "Going...", this_character());
        this_character()->handle_go(input, "teleport", "away");
        this_character()->describe_environment();
    }
}