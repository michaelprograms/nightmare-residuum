inherit STD_COMMAND;

void create () {
    ::create();
    set_syntax("unalias [name]");
    set_help_text("Remove an alias by name. Use " + format_syntax("alias") + " to view your current aliases.");
    set_help_similar(({ "alias" }));
}

void command (string input, mapping flags) {
    object tu = this_user();
    mapping alias;

    if (!input || !sizeof(input)) {
        message("action", "Usage: unalias [name]", tu);
        return;
    }

    alias = tu->query_alias(input);
    if (!alias) {
        message("action", "Alias '" + input + "': not found.", tu);
        return;
    }

    tu->unset_alias(input);
    tu->save_data();
    message("action", "Alias '" + input + "' removed.", tu);
}
