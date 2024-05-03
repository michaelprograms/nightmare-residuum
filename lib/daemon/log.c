int log_unique (string filename, string entry) {
    string *lines;

    if (!filename || filename == "" || !entry || entry == "") {
        return 0;
    }

    filename = replace_string(filename, " ", "_");
    if (!regexp(filename, "^/log/unique/")) {
        filename = "/log/unique/" + filename;
    }
    if (regexp(entry, "\n$")) {
        entry = entry[0..<2];
    }
    mkdirs(filename + ".txt"); // dummy extension

    lines = explode(read_file(filename) || "", "\n");
    if (member_array(entry, lines) > -1) {
        return 0;
    }
    return write_file(filename, entry + "\n");
}

int log (string filename, string entry) {
    if (!filename || filename == "" || !entry || entry == "") {
        return 0;
    }

    filename = replace_string(filename, " ", "_");
    if (!regexp(filename, "^/log/")) {
        filename = "/log/" + filename;
    }
    if (!regexp(entry, "\n$")) {
        entry += "\n";
    }
    mkdirs(filename + ".txt"); // dummy extension

    if (file_size(filename) >= 20000) { // 20 kb
        rename(filename, filename + "-" + time());
    }
    return write_file(filename, entry);
}