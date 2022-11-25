inherit STD_COMMAND;

void create () {
    set_syntax("objects (all|[name filter]|[file filter])");
    set_help_text("The objects command is used to view the list of objects in your current environment.\n\nUsing the all argument will show the entire list of objects.\n\nUsing a name or file filter will show the list of objects that match either filter.");
}

nosave private object *obs;

varargs string *tree (object ob, int indent, mapping b, int index, int maxIndex, mapping prefix) {
    string *result = ({ }), tmp = "";
    object *contents = ob->query_contents();
    int l = sizeof(contents);

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
    tmp += index + " " + (!indent && ob->inventory_accessible() ? "%^UNDERLINE%^BOLD%^" : "") + (ob->query_cap_name() ? ob->query_cap_name()+" ": "") + file_name(ob)+ (!indent ? "%^RESET%^" : "");
    obs -= ({ ob });

    result += ({ tmp });
    indent ++;
    if (ob->is_character()) {
        result += tree(ob->query_user(), indent, b, 0, 0, prefix);
        result += tree(ob->query_user()->query_shell(), indent+1, b, 1, 2, prefix);
        result += tree(ob->query_user()->query_account(), indent+1, b, 2, 2, prefix);
    }
    if (ob->is_vendor()) {
        result += tree(ob->query_vendor_inventory(), indent, b, 1, sizeof(ob->query_vendor_inventory()), prefix);
    }

    for (int i = 0; i < l; i ++) {
        result += tree(contents[i], indent, b, i+1, l, prefix);
    }

    return result;
}

void command (string input, mapping flags) {
    mapping b = query_border_charset();
    object *blueprints, *rooms, *containers;
    string *list = ({ });
    int l, n;

    if (!input) {
        obs = ({ environment(this_character()) });
    } else if (input == "all") {
        obs = objects();
    } else {
        obs = filter_array(objects(), (: $1 && ($1->query_key_name() == $(input) || regexp(file_name($1), $(input))) :));
    }

    rooms = filter_array(obs, (: $1 && $1->is_room() :));
    obs -= rooms;
    blueprints = filter_array(obs, (: $1 && !environment($1) && !clonep($1) && !regexp(file_name($1), "#") :));
    obs -= blueprints;

    rooms = sort_array(rooms, (: strcmp(base_name($1), base_name($2)) :));
    l = sizeof(rooms);
    for (int i = 0; i < l; i ++) {
        list += tree(rooms[i], 0, b, i+1, l, ([ ]));
    }
    n += l;

    containers = filter_array(obs, (: $1 && $1->inventory_accessible() :));
    l = sizeof(containers);
    for (int i = 0; i < l; i ++) {
        list += tree(containers[i], 0, b, n+i+1, n+l, ([ ]));
    }
    n += l;

    l = sizeof(obs);
    for (int i = 0; i < l; i ++) {
        list += tree(obs[i], 0, b, n+i+1, n+l, ([ ]));
    }

    border(([
        "title": "OBJECTS2",
        "subtitle": input,
        "body": ([
            "items": list,
            "columns": 1,
        ]),
    ]));
}