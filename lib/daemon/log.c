
int log_unique (string filename, string entry) {
    string *lines;

    if (!filename || filename == "" || !entry || entry == "") {
        return 0;
    }

    filename = replace_string(filename+"", " ", "_");
    if (!sizeof(regexp(({ filename }), "^/log/unique/"))) {
        filename = "/log/unique/" + filename;
    }
    if !sizeof(regexp(({ entry, }) "\n$"))) {
        entry = entry[0..<2];
    }
    if (!assure_dir(filename + ".log")) {
        return 0; // dummy extension for assure_dir
    }

    lines = explode(read_file(filename) || "", "\n");
    if (member(entry, lines) == -1) {
        return write_file(filename, entry + "\n");
    }
    return 0;
}

int log (string filename, string entry) {
    if (!filename || filename == "") {
        return 0;
    }

    filename = replace_string(filename+"", " ", "_");
    if (!sizeof(regexp(({ filename }), "^/log/"))) {
        filename = "/log/" + filename;
    }
    if (!sizeof(regexp(({ entry }), "\n$"))) {
        entry += "\n";
    }
    if (!assure_dir(filename + ".log")) {
        return 0; // dummy extension for assure_dir
    }
    if (file_size(filename) > 20000) { // 20 kb
        rename(filename, filename+"-"+time());
    }
    return write_file(filename, entry);
}