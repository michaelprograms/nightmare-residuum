inherit STD_COMMAND;

void create () {
    set_syntax("objects (all|[name filter]|[file filter])");
    set_help_text("The objects command is used to view the list of objects in your current environment.\n\nUsing the all argument will show the entire list of objects.\n\nUsing a name or file filter will show the list of objects that match either filter.");
}

mapping format_data (object ob) {
    mapping result = ([ ]);
    object *contents;
    int l;

    if (!ob) return 0;

    if (ob->is_character()) {
        result[file_name(ob->query_user())] = format_data(ob->query_user());
    }
    if (ob->is_user()) {
        result[file_name(ob->query_shell())] = format_data(ob->query_shell());
        result[file_name(ob->query_account())] = format_data(ob->query_account());
    }
    if (ob->is_vendor()) {
        result[file_name(ob->query_vendor_inventory())] = format_data(ob->query_vendor_inventory());
    }
    contents = ob->query_contents();
    l = sizeof(contents);
    for (int i = 0; i < l; i ++) {
        result[contents[i]->query_cap_name() + " " + file_name(contents[i])] = format_data(contents[i]);
    }

    return result;
}

void command (string input, mapping flags) {
    object *obs, *blueprints, *rooms, *containers;
    int l;

    mapping data = ([ ]);

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
        data[rooms[i]] = format_data(rooms[i]);
    }

    containers = filter_array(obs, (: $1 && $1->inventory_accessible() :));
    l = sizeof(containers);
    for (int i = 0; i < l; i ++) {
        data[containers[i]] = format_data(containers[i]);
    }

    l = sizeof(obs);
    for (int i = 0; i < l; i ++) {
        data[obs[i]] = format_data(obs[i]);
    }

    border(([
        "title": "OBJECTS",
        "subtitle": input,
        "body": ({
            ([
                "items": tree(data),
                "columns": 1,
            ])
        }),
    ]));
}