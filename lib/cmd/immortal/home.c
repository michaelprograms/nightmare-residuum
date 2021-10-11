void command (string input) {
    if (!input) {
        input = "/realm/" + this_character()->query_key_name() + "/workroom.c";
    } else {
        input = "/realm/" + input + "/workroom.c";
    }
    this_character()->handle_go(input, "teleport", "away");
    this_character()->describe_environment();
}