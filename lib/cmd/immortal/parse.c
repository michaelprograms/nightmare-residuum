inherit STD_COMMAND;

void create () {
    ::create();
    set_syntax("parse");
    set_help_text("The parse command can be used by an immortal to see debug information about how a verb is being parsed.");
}


void command (string input, mapping flags) {
    if (!input) {
        write("parse: input required.\n");
        return;
    }
    write("Sending '"+input+"' to parser...\n");
    this_character()->handle_command(input, 2);
}