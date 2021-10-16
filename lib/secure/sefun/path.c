string user_path (string name) {
    return "/realm" + (name && name != "" ? "/" + name : "");
}

string *split_path (string path) {
    int pos;
    while(path[<1] == '/' && strlen(path) > 1) path = path[0..<2];
    pos = strsrch(path, '/', -1);
    return ({ path[0..pos], path[pos+1..] });
}

string base_path (string path) {
    return split_path(path)[0];
}

// Evaluate . and .. and enforce leading /
string sanitize_path (string path) {
    string *parts;
    int i = 0, domain = 0; // , realm = 0;

    if (!path || (path[0] != '/' && path[0] != '~' && path[0] != '^')) {
        if (previous_object() && previous_object()->query_shell()) {
            path = previous_object()->query_shell()->query_variable("cwd");
        }
    }

    if (path[0] == '^') {
        path = replace_string(path, "^", "domain");
        domain = 1;
    } // else if (path[0] == "~") {
        // @TODO
    // }
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
        // if (!domain && !realm && tmp[0] == '~' && previous_object()->query_shell()) {
        //     realm = 1;
        //     if (sizeof(tmp) == 1) {
        //         tmp = previous_object()->query_character()->query_key_name();
        //     } else {
        //         tmp = tmp[1..];
        //     }
        //     parts[0..i] = explode(user_path(tmp), "/");
        // }
        i ++;
    }

    return "/" + implode(parts, "/") + (path[<1] == '/' || domain /* || realm */ ? "/" : "");
}

varargs string absolute_path (string relative_path, mixed relative_to) {
    if (!relative_to) relative_to = previous_object();
    if (relative_path[0] != '/') {
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
        if (file_size(dir = dir + "/" + dirs[i]) == -1) {
            check = D_ACCESS->unguarded((: mkdir, dir :));
        }
    }
    return check;
}