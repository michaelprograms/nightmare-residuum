inherit STD_COMMAND;

void create () {
    ::create();
    set_syntax("home [immortal]");
    set_help_text("The home command can be used by an immortal to return to their personal workroom, or the workroom of the named immortal if it exists.");
}

void command (string input, mapping flags) {
    if (!input) {
        input = this_character()->query_key_name();
    }
    input = "/realm/" + input + "/workroom.c";
    if (file_size(input) < 1) {
        message("action", "home: "+input+" not found.", this_character());
        return;
    }
    this_character()->handle_go(input, "teleport", "away");
    this_character()->describe_environment();
}