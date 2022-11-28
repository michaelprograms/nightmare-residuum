void command (string input, mapping flags) {
    string path;

    if (!input) {
        message("action", "Syntax: man [function]", this_character());
        return;
    }

    foreach (string type in ({ "apply", "efun", "lpc" })) {
        if (file_size(path = "/doc/" + type + "/" + input) > 0) {
            this_user()->handle_pager(read_file(path));
            return;
        }
    }

    message("action", "No such man page found.", this_character());
}