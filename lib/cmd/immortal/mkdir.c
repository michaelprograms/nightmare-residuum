void command (string input) {
    string cwd, dir;
    if (!input) {
        write("Syntax: mkdir [directory]\n");
    }

    cwd = this_user()->query_shell()->query_variable("cwd");
    dir = absolute_path(input, cwd);
    if (file_size(dir) != -1) {
        write("mkdir: " + dir + " already exists.\n");
        return;
    }

    write("mkdir: " + dir + " " + (mkdir(dir) ? "created" : " failed to create.") + ".\n");
}