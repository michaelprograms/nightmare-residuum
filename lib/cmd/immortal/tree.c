mapping localtree (string file, string fn, int index, int maxIndex) {
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
        tmp = localtree(inherits[i], fn, i, l);
        result[key] += tmp;
    }

    return result;
}

void command (string input, mapping flags) {
    string file, fn;
    mapping data = ([ ]);
    string subtitle;

    if (!input) {
        message("action", "Syntax: tree -fn=function [file]", this_user());
        return;
    }
    file = absolute_path(input, this_user()->query_shell()->query_variable("cwd"));
    if (file_size(file) < 1) {
        message("action", "tree: no such file.", this_user());
        return;
    }
    fn = flags["fn"];

    if (fn) {
        subtitle = "Searching for '" + fn + "'";
    }

    data = localtree(file, fn, 0, 1);

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