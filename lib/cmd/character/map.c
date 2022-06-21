void command (string input, mapping flags) {
    string *map;
    write("Map!\n");
    if (map = environment(this_character())->query_map("all")) {
        message("room_map", implode(map, "\n") + "\n", this_character());
    } else {
        write("No map to show.\n");
    }
}