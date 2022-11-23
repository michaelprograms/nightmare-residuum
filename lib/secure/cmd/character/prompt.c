inherit STD_COMMAND;

void create () {
    set_syntax("prompt ([format])");
    set_help_text("The prompt command is used to set the text that appears before each input you type.");
}

void command (string input, mapping flags) {
    object shell = this_user()->query_shell();

    if (!input) {
        write("Prompt is currently set to '" + shell->query_variable("prompt") + "'.\n");
        message("raw ansi", shell->query_variable("prompt") + "\n", this_character());
    } else {
        write("Changing prompt from '" + shell->query_variable("prompt") + "' to '" + input + "'.\n");
        shell->set_variable("prompt", input);
    }
}