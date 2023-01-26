
int log_unique (string filename, string entry) {
    string *lines;

    if (!filename || filename == "" || !entry || entry == "") {
        return 0;
    }

    filename = replace_string(filename+"", " ", "_");
    if (!regexp(filename, "^/log/unique/")) {
        filename = "/log/unique/" + filename;
    }
    if (regexp(entry, "\n$")) {
        entry = entry[0..<2];
    }
    if (!mkdirs(filename)) {
        return 0;
    }

    lines = explode(read_file(filename) || "", "\n");
    if (member_array(entry, lines) == -1) {
        return write_file(filename, entry + "\n");
    }
    return 0;
}

int log (string filename, string entry) {
    if (!filename || filename == "") {
        return 0;
    }

    filename = replace_string(filename+"", " ", "_");
    if (!regexp(filename, "^/log/")) {
        filename = "/log/" + filename;
    }
    if (!regexp(entry, "\n$")) {
        entry += "\n";
    }
    if (!mkdirs(filename)) {
        return 0;
    }
    if (file_size(filename) > 20000) { // 20 kb
        rename(filename, filename+"-"+time());
    }
    return write_file(filename, entry);
}