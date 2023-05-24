inherit STD_COMMAND;

void create () {
    ::create();
    set_syntax("prompt ([format])");
    set_help_text("The prompt command is used to set the text that appears before each input you type.");
}

void command (string input, mapping flags) {
    object shell = this_user()->query_shell();

    if (!input) {
        message("action", "Prompt is currently set to '" + shell->query_variable("prompt") + "'.", this_character());
        message("raw ansi", shell->query_variable("prompt") + "\n", this_character());
    } else {
        message("action", "Changing prompt from '" + shell->query_variable("prompt") + "' to '" + input + "'.", this_character());
        shell->set_variable("prompt", input);
    }
}