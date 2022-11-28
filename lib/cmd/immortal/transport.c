void command (string input, mapping flags) {
    object ob;

    if (!input) {
        message("action", "Transport who?", this_character());
        return;
    }

    if (ob = find_character(input)) {
        message("action", "Transporting "+ob->query_cap_name()+"...", this_character());
        message("action", "You are transported away...", ob);
        ob->handle_go(environment(this_character()), "teleport", "away");
        ob->describe_environment();
    } else {
        message("action", "transport: " + input + " not found.", this_character());
    }
}