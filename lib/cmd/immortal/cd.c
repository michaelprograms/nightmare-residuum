inherit STD_COMMAND;

void create () {
    ::create();
    set_syntax("cd [directory]");
    set_help_text("The cd command is used to change an immortal's working directory.");
}

void command (string input, mapping flags) {
    if (!input) {
        input = user_path(this_character()->query_key_name());
    }
    if (input[0] != '/' && input[0] != '~' && input[0] != '^') {
        input = this_user()->query_variable("cwd") + "/" + input;
    }
    input = sanitize_path(input);

    if (file_size(input) == -2) {
        this_user()->set_variable("cwd", input);
        write(input + ":\n");
    } else {
        write("Invalid directory.\n");
    }
}