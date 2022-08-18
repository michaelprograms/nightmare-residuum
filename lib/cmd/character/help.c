void command (string input, mapping flags) {
    string path;
    object file;

    if (!input) {
        message("action", "Syntax: help [command]\n", this_character());
        return;
    }

    if (path = D_COMMAND->query_ability(input)) {
        path += "/" + input + ".c";
        file = find_object(path) || load_object(path);
        if (function_exists("help", file)) {
            call_other(path, "help", this_character());
            return;
        }
    }

    message("action", "No help page found for that topic.\n", this_character());
}