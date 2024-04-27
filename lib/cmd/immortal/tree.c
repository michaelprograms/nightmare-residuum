inherit STD_COMMAND;

void create () {
    ::create();
    set_syntax("tree (-fn=function) [file]|(-file=file) [dir]");
    set_help_text("The tree command can be used to view a directory structure, or an object's inheritables. Search flags can also be passed.");
}

mapping tree_file (string file, string fn, int index, int maxIndex) {
    mapping result = ([ ]), tmp;
    string *inherits, err, key;
    object ob;
    int l, searchFlag = 0;

    if (!file) {
        return 0;
    }

    ob = find_object(file);
    if (!ob) {
        err = catch (ob = load_object(file));
        if (err) return 0;
    }

    l = sizeof(inherits = inherit_list(ob));
    searchFlag = (stringp(fn) && function_exists(fn, ob) + ".c" == file);

    if (searchFlag) {
        key = index + ". %^CYAN%^" + file + "%^RESET%^ <- " + fn;
    } else {
        key = index + ". " + file;
    }

    if (!mapp(result[key])) {
        result[key] = ([ ]);
    }
    for (int i = 0; i < l; i ++) {
        tmp = tree_file(inherits[i], fn, i, l);
        result[key] += tmp;
    }

    return result;
}

mapping tree_directory (string file, string search, int index, int maxIndex) {
    mapping result = ([ ]), tmp;
    string *files, key;
    int l, searchFlag = 0;

    if (!file) {
        return 0;
    }

    l = sizeof(files = get_dir(file + "/"));
    searchFlag = stringp(search) && regexp(file, search);

    if (searchFlag) {
        key = index + ". %^CYAN%^" + file + "%^RESET%^ <- " + search;
    } else {
        key = index + ". " + file;
    }

    if (!mapp(result[key])) {
        result[key] = ([ ]);
    }
    for (int i = 0; i < l; i ++) {
        tmp = tree_directory(file + "/" + files[i], search, i, l);
        result[key] += tmp;
    }

    return result;
}

void command (string input, mapping flags) {
    string file, tmp;
    mapping data = ([ ]);
    string subtitle;

    if (!input) {
        message("action", "Syntax: tree (-fn=function) [file]|(-file=file) [dir]", this_user());
        return;
    }

    file = absolute_path(input, this_user()->query_shell()->query_variable("cwd"));
    if (file[<1] == '/') {
        file = file[0..<2];
    }
    switch (file_size(file)) {
    case -2:
        if (tmp = flags["file"]) {
            subtitle = "Searching for '" + tmp + "'";
        }
        data = tree_directory(file, tmp, 0, 1);
        break;
    case -1:
        message("action", "tree: no such file.", this_user());
        return;
    case 0:
        message("action", "tree: file is empty.", this_user());
        return;
    default:
        if (tmp = flags["fn"]) {
            subtitle = "Searching for '" + tmp + "'";
        }
        data = tree_file(file, tmp, 0, 1);
        break;
    }

    border(([
        "title": "TREE",
        "subtitle": subtitle,
        "body": ({
            ([
                "items": tree(data),
                "columns": 1,
            ])
        }),
    ]));
}