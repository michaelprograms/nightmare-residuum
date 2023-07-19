inherit STD_COMMAND;

void create () {
    ::create();
    set_syntax("recall");
    set_help_text("The recall command is used to return the Origin point.");
}

void teleport (object tc) {
    if (!tc) return;

    tc->handle_go("/domain/Origin/room/center.c", "teleport", "away");
}

void command (string input, mapping flags) {
    object tc = this_character();

    if (sizeof(tc->query_present_hostiles())) {
        message("action", "You cannot recall while hostiles opponents are nearby.", tc);
        return;
    }
    if (base_name(environment(tc)) == "/domain/Origin/room/center") {
        message("action", "You are at the Origin recall point already.", tc);
        return;
    }
    message("go", "You begin to teleport...", tc);
    message("go", tc->query_cap_name() + " begins to teleport...", environment(tc), tc);
    call_out((: teleport :), 2, tc);
}