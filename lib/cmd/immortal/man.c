inherit STD_COMMAND;

void create () {
    ::create();
    set_syntax("man [apply|efun|lpc]");
    set_help_text("The man command is used to view driver documentation for the provided apply, efun, or LPC topic.");
}

void command (string input, mapping flags) {
    string path;

    if (!input) {
        message("action", "Syntax: man [function]", this_character());
        return;
    }

    // @TODO need to recursively look through directories for appropriate doc
    foreach (string type in ({ "apply", "efun", "lpc" })) {
        if (file_size(path = "/doc/" + type + "/" + input) > 0) {
            this_user()->handle_pager(read_file(path));
            return;
        }
    }

    message("action", "No such man page found.", this_character());
}