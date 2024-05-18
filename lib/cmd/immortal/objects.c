inherit STD_COMMAND;

mapping format_data (object ob);

void create () {
    set_syntax("objects (all|[name filter]|[file filter])");
    set_help_text("The objects command is used to view the list of objects in your current environment.\n\nUsing the all argument will show the entire list of objects.\n\nUsing a name or file filter will show the list of objects that match either filter.");
}

private void set_data (mapping result, object ob, int index) {
    mapping tmp = format_data(ob);
    string key;

    if (!ob) return;
    key = file_name(ob);
    if (ob->query_cap_name()) {
        key = ob->query_cap_name() + " " + key;
    }
    if (sizeof(tmp)) {
        key = "%^UNDERLINE%^" + key + "%^RESET%^";
    }
    result[key] = tmp;
}
private mapping format_data (object ob) {
    mapping result = ([ ]);
    object *contents = ({ });
    int l;

    if (!ob) return 0;

    if (ob->is_character()) {
        contents += ({ ob->query_user() });
    }
    if (ob->is_vendor()) {
        contents += ob->query_vendor_inventory();
    }
    if (ob->query_contents()) {
        contents += ob->query_contents();
    }

    l = sizeof(contents);
    for (int i = 0; i < l; i ++) {
        set_data(result, contents[i], i);
    }

    return result;
}
private void format_type (mapping data, object *obs) {
    int i, l;
    string key;
    mapping tmp;

    l = sizeof(obs);
    for (i = 0; i < l; i ++) {
        tmp = format_data(obs[i]);
        key = file_name(obs[i]);
        if (sizeof(tmp)) {
            key = "%^UNDERLINE%^" + key + "%^RESET%^";
        }
        data[key] = tmp;
    }
}

void command (string input, mapping flags) {
    object *obs, *rooms, *containers;
    mapping data = ([ ]);

    if (!input) {
        obs = ({ environment(this_character()) });
    } else if (input == "all") {
        obs = objects();
    } else {
        obs = filter(objects(), (: $1 && ($1->query_key_name() == $(input) || regexp(file_name($1), $(input))) :));
    }

    // format rooms first
    rooms = filter(obs, (: $1 && $1->is_room() :));
    rooms = sort_array(rooms, (: strcmp(base_name($1), base_name($2)) :));
    obs -= rooms;
    format_type(data, rooms);

    // format containers next
    containers = filter(obs, (: $1 && $1->inventory_accessible() :));
    obs -= containers;
    format_type(data, obs);

    // format rest of obs
    format_type(data, obs);

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