inherit STD_COMMAND;

void create () {
    ::create();
    set_syntax("tail (-n=15) [file]");
    set_help_text("The tail command can be used to view the last n (default 15) lines of the specified file.");
}

void command (string input, mapping flags) {
    string cwd, file, *lines;
    int n = 15;

    if (!input) {
        message("action", "Syntax: tail (-n=15) [file]", this_character());
        return;
    } else if (input == "here") {
        input = file_name(environment(this_character())) + ".c";
    }
    cwd = this_user()->query_shell()->query_variable("cwd");
    file = absolute_path(input, cwd);
    switch (file_size(file)) {
        case -2:
            message("action", "tail: " + file + ": not a file.", this_character());
            return;
        case -1:
            message("action", "tail: " + file + ": no such file.", this_character());
            return;
    }

    if (flags["n"]) n = to_int(flags["n"]);

    lines = explode(read_file(file), "\n");
    lines = lines[<n..];
    this_user()->handle_pager(lines);
}