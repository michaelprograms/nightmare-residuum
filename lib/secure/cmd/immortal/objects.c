string *sort_instance (string *list) {
    return sort_array(list, (: explode($1, "#")[<1] > explode($2, "#")[<1] :));
}
string format_object (string path, string *children, int indent) {
    string line = sprintf("%"+sprintf("%d", indent * 2)+"s", " ") + path + "\n";
    foreach (string c in sort_instance(children)) {
        line += sprintf("%"+sprintf("%d", (indent + 1) * 2)+"s", " ") + c + "\n";
    }
    return line;
}

void command (string input, mapping flags) {
    object *obs = filter_array(objects(), (:$1 && regexp(file_name($1), "#"):));
    mapping structure = ([]);

    write(format_header_bar("OBJECTS", mud_name()) + "\n\n");
    write("Spawned objects:\n");
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
        write(format_object(path, structure[path], 0));
    }

    write("\n" + format_footer_bar() + "\n");
}