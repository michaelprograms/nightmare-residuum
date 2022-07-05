varargs string *tree (string file, int indent, string fn, mapping b, int index, int maxIndex);

private nosave mapping __Border = ([
    "utf-8": ([
        // lines
        "h": "─",
        "v": "│",
        // corners
        "bl": "└",
        // joints
        "l": "├",
    ]),
    "US-ASCII": ([
        // lines
        "h": "-",
        "v": "|",
        // corners
        "bl": "'",
        // joints
        "l": "|",
    ]),
]);

void command (string input, mapping flags) {
    string file, fn;
    mapping b = __Border[query_encoding()];

    if (!input) {
        message("system", "Syntax: tree -fn=function [file]\n", this_user());
        return;
    }
    file = absolute_path(input, this_user()->query_shell()->query_variable("cwd"));
    if (file_size(file) < 1) {
        message("system", "tree: no such file.\n", this_user());
        return;
    }
    fn = flags["fn"];

    if (fn) {
        message("system", "Tree of " + file + " searching for '" + fn + "':\n\n", this_user());
    } else {
        message("system", "Tree of " + file + "\n\n", this_user());
    }
    foreach (string line in tree(file, 0, fn, b, 0, 0, ([ ]))) {
        message("system", line + "\n", this_user());
    }
    message("system", "\n", this_user());
}

varargs string *tree (string file, int indent, string fn, mapping b, int index, int maxIndex, mapping prefix) {
    string *result = ({ }), tmp = "", *inherits, err;
    object ob;
    int l, searchFlag = 0;

    if (!file) {
        return ({ "tree: file not found." });
    }

    ob = find_object(file);
    if (!ob) {
        err = catch (ob = load_object(file));
        if (err) return ({ "tree: unable to load " + file + ": " + err });
    }
    if (!ob) result += ({ "tree: " + file + " not found." });

    l = sizeof(inherits = inherit_list(ob));
    searchFlag = (stringp(fn) && function_exists(fn, ob) + ".c" == file);

    if (index > 0 && index == maxIndex) {
        prefix[indent-1] = " ";
    }

    if (indent > 0) {
        for (int i = 0; i < indent-1; i ++ ) {
            if (prefix[i]) tmp += prefix[i] + "  ";
            else tmp += b["v"] + "  ";
        }
        tmp += (index == maxIndex ? b["bl"] : b["l"]) + b["h"] + " ";
    }
    if (searchFlag) {
        tmp += "%^CYAN%^" + file + "%^RESET%^ <- " + fn;
    } else {
        tmp += file;
    }
    result += ({ tmp });

    indent ++;
    for (int i = 0; i < l; i ++) {
        result += tree(inherits[i], indent, fn, b, i+1, l, prefix);
    }

    return result;
}
