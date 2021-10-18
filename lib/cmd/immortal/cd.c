void command (string input) {
    if (!input) {
        input = user_path(this_character()->query_key_name());
    }
    if (input[0] != '/' && input[0] != '~' && input[0] != '^') {
        input = this_user()->query_shell()->query_variable("cwd") + "/" + input;
    }
    input = sanitize_path(input);
    write("Path is: "+input+"\n");
    if (file_size(input) == -2) {
        this_user()->query_shell()->set_variable("cwd", input);
        write(input + ":\n");
    } else {
        write("Invalid directory.\n");
    }
}