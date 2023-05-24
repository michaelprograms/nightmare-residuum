inherit STD_COMMAND;

void create () {
    ::create();
    set_syntax("shutdown");
    set_help_text("The shutdown command is used to stop the game server.");
}

void command (string input, mapping flags) {
    message("system", "Everything is suddenly nothing as irreality takes control.\n", characters());
    users()->quit_account();
    shutdown();
}