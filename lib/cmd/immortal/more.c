void command (string input, mapping flags) {
    string cwd, file, *lines;

    if (!input) {
        message("action", "Syntax: more [file]", this_character());
        return;
    } else if (input == "here") {
        input = file_name(environment(this_character())) + ".c";
    }
    cwd = this_user()->query_shell()->query_variable("cwd");
    file = absolute_path(input, cwd);
    switch (file_size(file)) {
        case -2:
            message("action", "more: " + file + ": not a file.", this_character());
            return;
        case -1:
            message("action", "more: " + file + ": no such file.", this_character());
            return;
    }

    lines = explode(read_file(file), "\n");
    this_user()->handle_pager(lines);
}