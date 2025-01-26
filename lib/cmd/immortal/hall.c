inherit STD_COMMAND;

void create () {
    ::create();
    set_syntax("hall");
    set_help_text("The hall command can be used by an immortal to return to the Immortal Hall.");
}

void command (string input, mapping flags) {
    this_character()->handle_go("/domain/Nowhere/room/hall.c", "teleport", "away");
}