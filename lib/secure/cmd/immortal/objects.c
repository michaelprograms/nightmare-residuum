string *sort_instance (string *list) {
    return sort_array(list, (: explode($1, "#")[<1] > explode($2, "#")[<1] :));
}
string format_object (string path, string *children, int indent) {
    string line = sprintf("%"+sprintf("%d", indent * 2)+"s", " ") + path;
    foreach (string c in sort_instance(children)) {
        line += "\n" + sprintf("%"+sprintf("%d", (indent + 1) * 4)+"s", " ") + c;
    }
    return line;
}

void command (string input, mapping flags) {
    object *obs = filter_array(objects(), (:$1 && regexp(file_name($1), "#"):));
    mapping structure = ([ ]);
    string *border, *list = ({ });

    foreach (object ob in obs) {
        object parent;
        if (parent = ob->query_parent()) {
            if (!arrayp(structure[file_name(parent)])) {
                structure[file_name(parent)] = ({});
            }
            structure[file_name(parent)] += ({ file_name(ob) });
        } else {
            object env;
            if (env = environment(ob)) {
                if (!arrayp(structure[file_name(env)])) {
                    structure[file_name(env)] = ({});
                }
                structure[file_name(env)] += ({ file_name(ob) });
            } else {
                if (!arrayp(structure[file_name(ob)])) {
                    structure[file_name(ob)] = ({});
                }
            }
        }
    }
    foreach (string path in sort_instance(keys(structure))) {
        list += explode(format_object(path, structure[path], 0), "\n");
    }

    border = format_border(([
        "title": "OBJECTS",
        "subtitle": mud_name(),
        "body": ([
            "items": list,
            "columns": 1,
        ]),
        "footer": ([
            "items": ({ sizeof(list) + " object" + (sizeof(list) > 1 ? "s" : "") }),
            "columns": 1,
            "align": "center",
        ]),
    ]));
    foreach (string line in border) {
        message("system", line + "\n", this_character());
    }
}