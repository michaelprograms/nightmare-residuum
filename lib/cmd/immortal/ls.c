inherit STD_COMMAND;

void create () {
    ::create();
    set_syntax("ls (-a) (-l) ([path])");
    set_help_text("Attempts to list files matching provided path. The path can contain the '*' wildcard character to filter results.");
}

private void output_long (string dir, mixed *files) {
    string acc, loaded, filename;

    acc = D_ACCESS->query_allowed(previous_object(), "read_file", dir, "read") ? "r" : "-";
    acc += D_ACCESS->query_allowed(previous_object(), "write_file", dir, "write") ? "w" : "-";
    acc += member_array(dir[0..<2], D_COMMAND->query_paths()) > -1 ? "x" : "-";

    foreach (string file in files) {
        loaded = file[1] != -2 && find_object(dir + file[0]) ? "*" : "";
        filename = file[0] + (file[1] == -2 ? "/" : "");
        message("action", sprintf("%:-3s%:-5s%:-25s %8d  %s", loaded, acc, ctime(file[2]), file[1], filename), this_user());
    }
}
private void output_brief (string dir, mixed *files) {
    int widest = 0, w, width, cols, rows, max, i, index;
    string line;
    string *list = map(files, function (mixed *file, string dir) {
        if (file[1] == -2) return "      " + file[0] + "/";
        else {
            string tmp = find_object(dir + file[0]) ? "*" : " ";
            if (!file[1]) tmp += sprintf("   0 %s", file[0]);
            else if (file[1] >= 1024) tmp += sprintf("%4d %s", file[1]/1024, file[0]);
            else tmp += sprintf("  .%1d %s", to_int(file[1]/1024.0*10), file[0]);
            return tmp;
        }
    }, dir);

    width = this_user()->query_setting("width") - 2;
    foreach (string file in list) {
        if ((w = strlen(file)) > widest) widest = w;
    }
    if (widest > width / 3 - 3) {
        widest = width / 3 - 3;
    }
    cols = width / (widest + 3);
    max = sizeof(list);
    rows = max / cols;
    if (max % cols) {
        rows ++;
    }

    for (i = 0; i < rows; i ++) {
        line = "";
        for (int j = 0; j < cols; j ++) {
            index = (rows * j) + i;
            if (index < max) {
                line += sprintf("%:-"+(widest+3)+"s", list[index]);
            } else {
                line += sprintf("%:-"+(widest+3)+"s", "");
            }
        }
        message("action", line, this_character());
    }
}

void command (string input, mapping flags) {
    string path, dir;
    mixed *files = ({ });
    int fs, r;

    if (!input || input == "") {
        path = this_user()->query_variable("cwd");
    } else if (input[0] != '/' && input[0] != '~' && input[0] != '^') {
        path = this_user()->query_variable("cwd");
        path = sanitize_path(path + "/" + input);
    } else {
        path = sanitize_path(input);
    }

    fs = file_size(path);
    r = regexp(path, "\\*");
    if (!r && fs == -1) {
        message("action", path + ": directory does not exist.", this_character());
        return;
    } else if (fs == -2) {
        if (path[<1] != '/') {
            path += "/";
        }
        dir = path;
    } else {
        dir = path[0..strsrch(path, "/", -1)];
    }

    files = get_dir(path, -1) || ({ });
    if (!flags["a"]) {
        files = filter(files, (: $1[0][0] != '.' :));
    }

    if (!sizeof(files)) {
        message("action", path + ": " + (r ? "no matches" : "directory is empty.\n"), this_character());
    } else {
        message("action", path + ":", this_character());
    }

    if (flags["l"]) {
        output_long(dir, files);
    } else {
        output_brief(dir, files);
    }
}