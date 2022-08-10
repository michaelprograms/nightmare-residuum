string user_path (mixed *args...) {
    string name;
    if (sizeof(args)) name = args[0];
    return "/realm" + (name && name != "" ? "/" + name : "");
}

string *split_path (string path) {
    int pos;
    while (path[<1] == '/' && strlen(path) > 1) path = path[0..<2];
    pos = strsrch(path, '/', -1);
    return ({ path[0..pos], path[pos+1..] });
}

string base_path (string path) {
    return split_path(path)[0];
}

// Evaluate . and .. and enforce leading /
string sanitize_path (string path) {
    string *parts;
    int i = 0;
    int trailingSlash;

    if (undefinedp(path) || !path || path == "" || (path[0] != '/' && path[0] != '~' && path[0] != '^')) {
        if (previous_object() && previous_object()->query_shell()) {
            path = previous_object()->query_shell()->query_variable("cwd");
        }
    }

    trailingSlash = (strlen(path) > 0 && (path[<1] == '/' || path[<2..] == "/." || path[<3..] == "/.."));
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
                parts[i-1..i] = ({});
                i --;
            } else {
                parts[i..i] = ({});
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
    if (!relative_to) relative_to = previous_object();
    if (relative_path && relative_path[0] != '/' && relative_path[0] != '^' && relative_path[0] != '~') {
        if (objectp(relative_to)) {
            relative_path = base_path(file_name(relative_to)) + "/" + relative_path;
        } else if (stringp(relative_to)) {
            relative_path = relative_to + "/" + relative_path;
        } else {
            return "";
        }
    }
    relative_path = sanitize_path(relative_path);
    return relative_path;
}

// Check path recursively and create dirs
int assure_dir (string path) {
    string *dirs;
    string dir = "";
    int check = 1;

    if (!path) {
        return 0;
    }
    dirs = split_path(path);
    dirs = explode(dirs[0], "/") + (!regexp(dirs[1], "\\.") ? ({ dirs[1] }) : ({}));
    for (int i = 0; check && i < sizeof(dirs); i ++) {
        dir = dir + "/" + dirs[i];
        if (SEFUN->unguarded((: file_size, dir :)) == -1) {
            check = SEFUN->unguarded((: mkdir, dir :));
        }
    }
    return check;
}

string *wild_card (string path, string relative_to) {
    string cwd, *split, *match;

    if (!path || sizeof(path) < 1) {
        return ({});
    }
    cwd = absolute_path(path, relative_to);
    if (cwd == "/") {
        return ({ "/" });
    }
    split = split_path(cwd);
    if (split[0] == "/") {
        split[0] = "";
    }
    match = get_dir(cwd);
    if (!match) {
        match = ({});
    } else {
        match -= ({ "." });
        match -= ({ ".." });
    }
    if (!path || path[0] != '.') {
        match = filter_array(match, (: $1[0] != '.' :));
    }
    for (int i = 0; i < sizeof(match); i ++) {
        if (file_size(split[0] + split[1]) == -2) {
            match[i] = split[0];
        } else {
            match[i] = split[0] + match[i];
        }
        if (strlen(match[i]) > 1 && match[i][0..1] == "//") {
            match[i] = match[i][1..<1];
        }
    }
    return match;
}