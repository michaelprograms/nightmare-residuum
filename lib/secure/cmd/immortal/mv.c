inherit STD_COMMAND;

void create () {
    set_syntax("mv [file] [newfile]");
    set_help_text("The mv command is used to move a file to a new filename.");
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
    if (file2[<1] == '/') {
        file2 = file2[0..<2];
    }
    if (file_size(file2) == -2) {
        file2 += "/" + explode(file1, "/")[<1];
    }
    rename(file1, file2);
    write("mv " + (file_size(file1) < 0 && file_size(file2) != -1 ? "succeeded" : "failed") + ": " + file1 + " to "+ file2 + "\n");
}