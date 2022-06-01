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
    foreach (string line in tree(file, 0, fn, b)) {
        message("system", line + "\n", this_user());
    }
    message("system", "\n", this_user());
}

varargs string *tree (string file, int indent, string fn, mapping b, int index, int maxIndex) {
    string *result = ({ }), tmp = "", *inherits;
    object ob;
    int l, searchFlag = 0;

    if (!file) {
        return ({ "tree: file not found." });
    }

    ob = find_object(file);
    if (!ob) {
        tmp = catch (ob = load_object(file));
        if (tmp) return ({ "tree: unable to load " + file + ": " + tmp });
    }
    if (!ob) result += ({ "tree: " + file + " not found." });

    l = sizeof(inherits = inherit_list(ob));
    searchFlag = (stringp(fn) && function_exists(fn, ob) + ".c" == file);

    if (indent > 0) {
        for (int i = 0; i < indent-1; i ++ ) {
            tmp += b["v"] + "  ";
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
        result += tree(inherits[i], indent, fn, b, i+1, l);
    }

    return result;
}
