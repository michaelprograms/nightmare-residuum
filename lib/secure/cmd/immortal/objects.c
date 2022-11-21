inherit STD_COMMAND;

void create () {
    set_syntax("objects");
    set_help_text("The objects command is used to view the entire list of objects.");
}

string *sort_instance (string *list) {
    return sort_array(list, (: explode($1, "#")[<1] > explode($2, "#")[<1] :));
}
string format_object (string path, string *children, int indent) {
    string line = sprintf("%*s", indent*2, " ") + path;
    foreach (string c in sort_instance(children)) {
        line += "\n" + sprintf("%*s", (indent+1)*4, " ") + c;
    }
    return line;
}

void command (string input, mapping flags) {
    object *obs = filter_array(objects(), (:$1 && regexp(file_name($1), "#"):));
    mapping structure = ([ ]);
    string *list = ({ });

    foreach (object ob in obs) {
        object parent;
        string key;
        if (parent = ob->query_parent()) {
            key = (parent->query_cap_name() ? parent->query_cap_name()+" " : "") + file_name(parent);
            if (!arrayp(structure[key])) {
                structure[key] = ({ });
            }
            structure[key] += ({ (ob->query_cap_name() ? ob->query_cap_name()+" " : "") + file_name(ob) });
        } else {
            object env;
            if (env = environment(ob)) {
                key = (env->query_cap_name() ? env->query_cap_name()+" " : "") + file_name(env);
                if (!arrayp(structure[key])) {
                    structure[key] = ({ });
                }
                structure[key] += ({ (ob->query_cap_name() ? ob->query_cap_name()+" " : "") + file_name(ob) });
            } else {
                key = (ob->query_cap_name() ? ob->query_cap_name()+" " : "") + file_name(ob);
                if (!arrayp(structure[key])) {
                    structure[key] = ({ });
                }
            }
        }
    }
    foreach (string path in sort_instance(keys(structure))) {
        list += explode(format_object(path, structure[path], 0), "\n");
    }

    border(([
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
}