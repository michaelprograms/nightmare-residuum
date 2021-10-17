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
    int i = 0;
    int trailingSlash;

    if (undefinedp(path) || !path || path == "" || (path[0] != '/' && path[0] != '~' && path[0] != '^')) {
        if (previous_object() && previous_object()->query_shell()) {
            path = previous_object()->query_shell()->query_variable("cwd");
        }
    }

    trailingSlash = (strlen(path) > 0 && path[<1] == '/');
    if (path[0] == '^') {
        path = replace_string(path, "^", "domain/");
        trailingSlash = 1;
    } else if (path[0] == '~') {
        // write("sanitize_path("+path+"): "+identify(previous_object())+" "+identify(previous_object()->query_character())+" "+identify(previous_object()->query_character()->query_key_name())+"\n");
        path = replace_string(path, "~", "realm/" + previous_object()->query_character()->query_key_name() + "/");
        trailingSlash = 1;
        // write("path is now: "+path+"\n");
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

    return "/" + implode(parts, "/") + (trailingSlash ? "/" : "");
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