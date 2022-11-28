inherit STD_COMMAND;

void create () {
    set_syntax("quit");
    set_help_text("The quit command is used to log out of your character.");
}

void command (string input, mapping flags) {
    if (!this_character()) {
        message("action", "Quitting out...", this_user());
    } else {
        message("action", "Quitting out of " + this_character()->query_cap_name() + ".", this_user());
    }
    this_user()->quit_character();
}