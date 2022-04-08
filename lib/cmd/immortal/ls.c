private void output_long (string dir, string path) { // mixed *files) {
    string acc, loaded, filename;

    acc = D_ACCESS->query_allowed(previous_object(), "read_file", dir, "read") ? "r" : "-";
    acc += D_ACCESS->query_allowed(previous_object(), "write_file", dir, "write") ? "w" : "-";
    acc += member_array(dir[0..<2], D_COMMAND->query_paths()) > -1 ? "x" : "-";

    foreach (string file in get_dir(path, -1)) {
        loaded = file[1] != -2 && find_object(dir + file[0]) ? "*" : "";
        filename = file[0] + (file[1] == -2 ? "/" : "");
        write(sprintf("%:-3s%:-5s%:-30s %d\t%s", loaded, acc, ctime(file[2]), file[1], filename)+"\n");
    }
}

void command (string input) {
    string path, dir;

    if (!input || input == "") {
        path = this_user()->query_shell()->query_variable("cwd");
    } else if (input[0] != '/') {
        path = this_user()->query_shell()->query_variable("cwd");
        path = sanitize_path(path + "/" + input);
    } else {
        path = sanitize_path(input);
    }

    if (file_size(path) == -2) {
        if (path[<1] != '/') {
            path += "/";
        }
        dir = path;
    } else {
        dir = path[0..strsrch(path, "/", -1)];
    }

    write(path + ":\n");
    output_long(dir, path);
}