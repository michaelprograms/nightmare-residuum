string user_path (mixed *args...) {
    string name;
    if (sizeof(args)) name = args[0];
    return "/realm" + (name && name != "" ? "/" + name : "");
}

string *split_path (string path) {
    int pos;
    while (path[<1] == '/' && sizeof(path) > 1) {
        path = path[0..<2];
    }
    pos = strsrch(path, '/', -1);
    return ({ path[0..pos], path[pos+1..] });
}

// Evaluate . and .. and enforce leading /
string sanitize_path (string path) {
    string *parts;
    int i = 0;
    int trailingSlash;

    if (!stringp(path) || !sizeof(path)) {
        error("Bad argument 1 to path->sanitize_path");
    }

    if ((path[0] != '/' && path[0] != '~' && path[0] != '^')) {
        if (previous_object() && previous_object()->query_shell()) {
            path = previous_object()->query_shell()->query_variable("cwd");
        }
    }

    trailingSlash = (sizeof(path) > 0 && (path[<1] == '/' || path[<2..] == "/." || path[<3..] == "/.."));
    if (path[0] == '^') {
        path = replace_string(path, "^", "domain/");
    } else if (path[0] == '~') {
        path = replace_string(path, "~", "realm/" + SEFUN->this_character()->query_key_name() + "/");
    }
    parts = explode(path, "/") - ({ "", "." });
    if (!sizeof(parts)) {
        return "/";
    }

    while (i < sizeof(parts)) {
        string tmp = parts[i];
        if (tmp == "..") {
            if (i) {
                parts[i-1..i] = ({ });
                i --;
            } else {
                parts[i..i] = ({ });
            }
            continue;
        }
        i ++;
    }

    if (!sizeof(parts) && trailingSlash) {
        trailingSlash = 0;
    }

    return "/" + implode(parts, "/") + (trailingSlash ? "/" : "");
}

varargs string absolute_path (string relative_path, mixed relative_to) {
    if (!relative_to || !(objectp(relative_to) || stringp(relative_to))) {
        relative_to = previous_object();
    }
    if (relative_path && member_array(relative_path[0], ({ '/', '^', '~' })) == -1) {
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
// Returns 1 for directory exists or 0 for failure
int mkdirs (string path) {
    string *dirs, dir = "";
    int i, l, check = 1;

    if (!stringp(path) || !sizeof(path)) {
        return 0;
    }

    dirs = explode(path, "/") - ({ "" });
    // dirs[<1] potentially contains a filename with extension
    dirs = dirs[0..<2] + (!regexp(dirs[<1], "\\.") ? ({ dirs[<1] }) : ({ }));
    path = "/" + implode(dirs, "/");
    if (file_size(path) == -2) {
        return 1;
    }

    l = sizeof(dirs);
    for (i = 0; check && i < l; i ++) {
        dir += "/" + dirs[i];
        switch (file_size(dir)) {
        case -2:
            continue;
        case -1:
            check = mkdir(dir);
            break;
        default:
            check = 0; // a file already exists with this path
            break;
        }
    }
    return check;
}

string *wild_card (string path) {
    string *match;

    if (!path || path == "") {
        return ({ });
    }

    path = absolute_path(path, "/");
    match = filter(get_dir(path) || ({ }), (: $1 != "." && $1 != ".." :));
    if (path[<1] != '/') {
        path = split_path(path)[0];
    }

    for (int i = 0; i < sizeof(match); i ++) {
        switch (file_size(path + match[i])) {
        case -2:
            match[i] = path + match[i];
            break;
        default:
            match[i] = path + match[i];
            break;
        }
    }
    return match;
}