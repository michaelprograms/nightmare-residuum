inherit STD_COMMAND;

void create () {
    ::create();
    set_syntax("prompt ([format])");
    set_help_text("The prompt command is used to set the text that appears before each input you type.");
}

void command (string input, mapping flags) {
    object tu = this_user();

    if (!input) {
        message("action", "Prompt is currently set to '" + tu->query_variable("prompt") + "'.", tu);
        message("raw ansi", tu->query_variable("prompt") + "\n", tu);
    } else {
        message("action", "Changing prompt from '" + tu->query_variable("prompt") + "' to '" + input + "'.", tu);
        tu->set_variable("prompt", input);
    }
}