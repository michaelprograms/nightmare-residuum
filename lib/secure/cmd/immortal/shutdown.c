inherit STD_COMMAND;

void create () {
    set_syntax("shutdown");
    set_help_text("The shutdown command is used to stop the game server.");
}

void command (string input, mapping flags) {
    message("system", "\n" + mud_name() + " is entering shutdown mode.\n\n", characters());
    shutdown();
}