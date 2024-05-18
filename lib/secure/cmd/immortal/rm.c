inherit STD_COMMAND;

void create () {
    ::create();
    set_syntax("rm [file]");
    set_help_text("The rm command is used to remove a file permanently.");
}

void command (string input, mapping flags) {
    string cwd, file;
    if (!input) {
        write("Syntax: rm [file]\n");
        return;
    }

    cwd = this_user()->query_variable("cwd");
    file = absolute_path(input, cwd);
    switch (file_size(file)) {
        case -2:
            write("rm: " + file + ": not a file.\n");
            return;
        case -1:
            write("rm: " + file + ": no such file.\n");
            return;
    }

    write("rm: " + file + " " + (rm(file) ? "removed" : " failed to remove") + ".\n");
}