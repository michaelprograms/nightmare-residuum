void command (string input, mapping flags) {
    string cwd, dir;
    if (!input) {
        write("Syntax: mkdir [directory]\n");
        return;
    }

    cwd = this_user()->query_shell()->query_variable("cwd");
    dir = absolute_path(input, cwd);
    if (file_size(dir) != -1) {
        write("mkdir: " + dir + " already exists.\n");
        return;
    }

    write("mkdir: " + dir + " " + (mkdir(dir) ? "created" : " failed to create.") + ".\n");
}