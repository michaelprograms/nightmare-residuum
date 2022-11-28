inherit STD_COMMAND;

void create () {
    set_syntax("idle [character]");
    set_help_text("The idle command is used to view how long ago a connected character last sent a command.");
}

void command (string input, mapping flags) {
    object tc = this_character(), target;

    if (!sizeof(input) || !(target = find_character(input))) {
        message("action", "Syntax: <idle [character]>", tc);
        return;
    }

    message("action", target->query_cap_name() + " has been idle for " + time_from_seconds(query_idle(target->query_user()), 2) + ".", tc);
}