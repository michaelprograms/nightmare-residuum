inherit STD_COMMAND;

void create () {
    ::create();
    set_syntax("prompt ([format])");
    set_help_text("The prompt command is used to set the text that appears before each input you type.\n\nAvailable tokens: " + format_syntax("$hp") + " current hp, " + format_syntax("$HP") + " max hp, " + format_syntax("$sp") + " current sp, " + format_syntax("$SP") + " max sp, " + format_syntax("$mp") + " current mp, " + format_syntax("$MP") + " max mp, " + format_syntax("$cd") + " current directory.\n\nExample: " + format_syntax("prompt $hp/$HP hp $sp/$SP sp >"));
    set_help_similar(({ "alias", "settings" }));
}

void command (string input, mapping flags) {
    object tu = this_user();

    if (!input) {
        message("action", "Prompt is currently set to '" + tu->query_variable("prompt") + "'.", tu);
    } else {
        message("action", "Changing prompt from '" + tu->query_variable("prompt") + "' to '" + input + "'.", tu);
        tu->set_variable("prompt", input);
    }
}