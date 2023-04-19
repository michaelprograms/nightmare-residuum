void command (string input, mapping flags) {
    string cwd, file, *lines;

    if (!input) {
        write("Syntax: cat [file]\n");
        return;
    }
    cwd = this_user()->query_shell()->query_variable("cwd");
    file = absolute_path(input, cwd);
    switch (file_size(file)) {
        case -2:
            write("cat: " + file + ": not a file.\n");
            return;
        case -1:
            write("cat: " + file + ": no such file.\n");
            return;
    }

    lines = explode(read_file(file), "\n");
    foreach (string line in lines) {
        write(line + "\n");
    }
}