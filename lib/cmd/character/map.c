inherit STD_COMMAND;

void create () {
    set_syntax("map");
    set_help_text("The map command is used to display the minimap for your character's current environment.");
}

void command (string input, mapping flags) {
    string *map;
    if (map = environment(this_character())->query_map("all")) {
        message("room_map", implode(map, "\n") + "\n", this_character());
    } else if (map = environment(this_character())->query_room_exit_map()) {
        message("room_map", implode(map, "\n") + "\n", this_character());
    } else {
        write("No map to show.\n");
    }
}