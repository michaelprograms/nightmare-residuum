inherit STD_COMMAND;

void create () {
    ::create();
    set_syntax("cp [file] [newfile]");
    set_help_text("The cp command is used to save a copy of a file to a new filename.");
}

void command (string input, mapping flags) {
    string file1, file2, cwd;

    if (!input || sscanf(input, "%s %s", file1, file2) != 2) {
        write("Syntax: mv [file_location] [new_location]\n");
        return;
    }

    cwd = this_user()->query_shell()->query_variable("cwd");
    file1 = absolute_path(file1, cwd);
    file2 = absolute_path(file2, cwd);
    if (file_size(file2) == -2) {
        file2 += "/" + explode(file1, "/")[<1];
    }
    cp(file1, file2);
    write("cp " + (file_size(file2) > -1 ? "succeeded" : "failed") + ": " + file1 + " to "+ file2 + "\n");
}