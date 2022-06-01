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
    object *obs = filter(objects(), (:$1 && regexp(program_name($1), "#"):));
    mapping structure = ([ ]);
    string *border, *list = ({ });

    foreach (object ob in obs) {
        object parent;
        if (parent = ob->query_parent()) {
            if (!pointerp(structure[program_name(parent)])) {
                structure[program_name(parent)] = ({});
            }
            structure[program_name(parent)] += ({ program_name(ob) });
        } else {
            object env;
            if (env = environment(ob)) {
                if (!pointerp(structure[program_name(env)])) {
                    structure[program_name(env)] = ({});
                }
                structure[program_name(env)] += ({ program_name(ob) });
            } else {
                if (!pointerp(structure[program_name(ob)])) {
                    structure[program_name(ob)] = ({});
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