
int log_unique (string filename, string entry) {
    string *lines;

    if (!filename || filename == "") {
        return 0;
    }

    filename = replace_string(filename+"", " ", "_");
    if (!regexp(filename, "^/log/unique/")) {
        filename = "/log/unique/" + filename;
    }
    if (regexp(entry, "\n$")) {
        entry = entry[0..<2];
    }
    if (!assure_dir(filename + ".log")) {
        return 0; // dummy extension for assure_dir
    }

    lines = explode(read_file(filename), "\n");
    if (member_array(entry, lines) == -1) {
        write_file(filename, entry);
        return 1;
    }
    return 0;
}