inherit STD_COMMAND;

void create () {
    ::create();
    set_syntax("rmdir [file]");
    set_help_text("The rmdir command is used to remove a directory permanently.");
}

void command (string input, mapping flags) {
    string cwd, dir;
    if (!input) {
        write("Syntax: rmdir [directory]\n");
        return;
    }

    cwd = this_user()->query_variable("cwd");
    dir = absolute_path(input, cwd);
    switch (file_size(dir)) {
        case -2: break;
        case -1:
            write("rmdir: " + dir + ": no such directory.\n");
            return;
        default:
            write("rmdir: " + dir + ": not a directory.\n");
            return;
    }

    write("rmdir: " + dir + " " + (rmdir(dir) ? "removed" : " failed to remove") + ".\n");
}