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
    string *parts = explode(path, "/") - ({ "", "." });
    int i;

    if (!sizeof(parts)) return "/";

    while ((i = member_array("..", parts)) != -1) {
	    if (i > 1) parts = parts[0..i-2] + parts[i+1..];
	    else if (i == 0) parts = parts[1..];
	    else parts = parts[2..];
    }

    // @TODO ~ and ^

    return "/" + implode(parts, "/") + (path[<1] == '/' ? "/" : "");
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