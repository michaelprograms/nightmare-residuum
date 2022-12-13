void command (string input, mapping flags) {
    string cwd, file, *lines;
    int n = 15;

    if (!input) {
        message("action", "Syntax: head (-n=15) [file]", this_character());
        return;
    } else if (input == "here") {
        input = file_name(environment(this_character())) + ".c";
    }
    cwd = this_user()->query_shell()->query_variable("cwd");
    file = absolute_path(input, cwd);
    switch (file_size(file)) {
        case -2:
            message("action", "head: " + file + ": not a file.", this_character());
            return;
        case -1:
            message("action", "head: " + file + ": no such file.", this_character());
            return;
    }

    if (flags["n"]) n = to_int(flags["n"]);

    lines = explode(read_file(file), "\n");
    lines = lines[0..<(sizeof(lines)-n+1)];
    this_user()->handle_pager(lines);
}