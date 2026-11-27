string user_path(mixed args...) {
    string name;
    if (sizeof(args)) {
        name = args[0];
    }
    return "/realm" + (name && name != "" ? "/" + name : "");
}

string *split_path(string path) {
    int pos;
    while (sizeof(path) > 1 && path[<1] == '/') {
        path = path[0..<2];
    }
    pos = strsrch(path, '/', -1);
    if (pos == -1) {
        return ({ "", path });
    }
    return ({ path[0..pos], path[pos + 1..] });
}

// Evaluate . and .. and enforce leading /
string sanitize_path(string path) {
    string *parts;
    int i = 0;
    int trailingSlash;
    string cwd;

    if (!stringp(path) || !sizeof(path)) {
        error("Bad argument 1 to path->sanitize_path");
    }

    if ((path[0] != '/' && path[0] != '~' && path[0] != '^') && previous_object()) {
        cwd = previous_object()->query_variable("cwd") || "";
        if (sizeof(cwd) && cwd[<1] != '/') {
            cwd += "/";
        }
        path = cwd + path;
    }

    trailingSlash = (path[<1] == '/' || path[<2..] == "/." || path[<3..] == "/..");
    if (path[0] == '^') {
        path = "domain/" + path[1..];
    } else if (path[0] == '~') {
        path = "realm/" + SEFUN->this_character()->query_key_name() + "/" + path[1..];
    }
    parts = explode(path, "/") - ({ "", "." });
    if (!sizeof(parts)) {
        return "/";
    }

    while (i < sizeof(parts)) {
        string tmp = parts[i];
        if (tmp == "..") {
            if (i) {
                parts[i - 1..i] = ({});
                i--;
            } else {
                parts[i..i] = ({});
            }
            continue;
        }
        i++;
    }

    if (!sizeof(parts) && trailingSlash) {
        trailingSlash = 0;
    }

    return "/" + implode(parts, "/") + (trailingSlash ? "/" : "");
}

varargs string absolute_path(string relative_path, mixed relative_to) {
    if (!stringp(relative_path)) {
        return 0;
    }
    if (!relative_to || !(objectp(relative_to) || stringp(relative_to))) {
        relative_to = previous_object();
    }
    if (member_array(relative_path[0], ({ '/', '^', '~' })) == -1) {
        if (objectp(relative_to)) {
            relative_path = split_path(file_name(relative_to))[0] + "/" + relative_path;
        } else {
            relative_path = relative_to + "/" + relative_path;
        }
    }
    relative_path = sanitize_path(relative_path);
    return relative_path;
}

// Check path recursively and create dirs
// Path can be a directory or filename (which will be truncated)
// A trailing / marks the final part as a directory even if it contains a dot
// Returns 1 for directory exists or 0 for failure
int mkdirs(string path) {
    string *dirs, dir = "";
    int i, l, check = 1;

    if (!stringp(path) || !sizeof(path)) {
        return 0;
    }

    dirs = explode(path, "/") - ({ "" });
    if (!sizeof(dirs)) {
        return 1;  // root directory
    }
    if (path[<1] != '/' && regexp(dirs[<1], "\\.")) {
        // dirs[<1] is a filename with extension
        dirs = dirs[0..<2];
    }
    path = "/" + implode(dirs, "/");
    if (file_size(path) == -2) {
        return 1;
    }

    l = sizeof(dirs);
    for (i = 0; check && i < l; i++) {
        dir += "/" + dirs[i];
        switch (file_size(dir)) {
            case -2:
                continue;
            case -1:
                check = mkdir(dir);
                break;
            default:
                check = 0;  // a file already exists with this path
                break;
        }
    }
    return check;
}

string *wild_card(string path) {
    string *match = ({});
    if (path && path != "") {
        path = absolute_path(path, "/");
        match = filter(get_dir(path) || ({}), (: $1 != "." && $1 != ".." :));
        if (path[<1] != '/') {
            path = split_path(path)[0];
        }
        match = map_array(match, (: $(path) + $1 :));
    }
    return match;
}

string query_file_recursive(string path, string type) {
    string *dirs, typePath;
    int l;
    dirs = explode(path, "/")[0..<2];
    l = sizeof(dirs);
    while (l--) {
        typePath = "/" + implode(dirs[0..l], "/") + "/" + type + ".c";
        if (file_size(typePath) > 0) {
            return typePath;
        }
    }
    return 0;
}
