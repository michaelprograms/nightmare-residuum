inherit STD_COMMAND;

void create () {
    set_syntax("map");
    set_help_text("The map command is used to display the minimap for your character's current environment.");
}

void command (string input, mapping flags) {
    object env = environment(this_character());
    string *map;
    if (map = env->query_map("all")) {
        message("action", implode(map, "\n"), this_user());
    } else if (map = env->query_room_exit_map()) {
        message("action", implode(map, "\n"), this_user());
    } else {
        message("action", "No map to show.", this_user());
    }
}