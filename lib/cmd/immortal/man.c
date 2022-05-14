void command (string input, mapping flags) {
    string path;

    if (!input) {
        message("action", "Syntax: man [function]\n", this_character());
        return;
    }

    foreach (string type in ({ "apply", "efun", "lpc" })) {
        if (file_size(path = "/doc/" + type + "/" + input) > 0) {
            foreach (string line in explode(read_file(path), "\n")) {
                message("action", line + "\n", this_character());
            }
            return;
        }
    }

    message("action", "No such man page found.\n", this_character());
}