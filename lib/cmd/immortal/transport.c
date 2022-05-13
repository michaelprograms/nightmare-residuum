void command (string input, mapping flags) {
    object ob;

    if (!input) {
        message("action", "Transport who?\n", this_character());
        return;
    }

    if (ob = find_character(input)) {
        message("action", "Transporting "+ob->query_cap_name()+"...\n", this_character());
        message("action", "You are transported away...\n", ob);
        ob->handle_go(environment(this_character()), "teleport", "away");
        ob->describe_environment();
    } else {
        message("action", "transport: " + input + " not found.\n", this_character());
    }
}