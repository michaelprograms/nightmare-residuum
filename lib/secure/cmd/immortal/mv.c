void command (string input) {
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
    rename(file1, file2);
    write("mv " + (file_size(file1) < 0 && file_size(file2) != -1 ? "succeeded" : "failed") + ": " + file1 + " to "+ file2 + "\n");
}