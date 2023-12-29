nosave private mapping __Exits = ([
// Data Format:
/*
    STRING: ([                  // Direction
        "room":     STRING,     // Path of room
        "before":   FUNCTION,   // before exit fn: 0 for failure, 1 for success
        "after":    FUNCTION,   // after exit fn
        "reverse":  STRING,     // override automatic direction reverse lookup
        "door":     STRING,     // ID of the door blocking exit
        "key":      STRING,     // ID of the key for door
        "open":     INT,        // door open status
        "locked":   INT,        // door locked status
    ])
*/
]);
nosave private mapping __Climbs = ([ ]);
nosave private int __HiddenExits = 0;

/* ----- exits ----- */

mapping query_exits () {
    return copy(__Exits);
}
string *query_exit_directions () {
    return keys(__Exits);
}
string *query_exit_dirs () { // shorthand directions
    string *exits = keys(__Exits);
    for (int i = 0; i < sizeof(exits); i ++) {
        exits[i] = format_exit_brief(exits[i]);
    }
    return exits;
}
string *query_exit_destinations () {
    return values(__Exits);
}
string query_exit (string dir) {
    dir = format_exit_verbose (dir);
    if (__Exits[dir]) {
        return __Exits[dir]["room"];
    } else if (__Exits["out " + dir]) {
        return __Exits["out " + dir]["room"];
    } else if (__Exits["enter " + dir]) {
        return __Exits["enter " + dir]["room"];
    }
    return 0;
}
string query_default_enter () {
    string *enters = filter(keys(__Exits), (: regexp($1, "^enter") :));
    return sizeof(enters) == 1 ? enters[0] : 0;
}
string query_default_out () {
    string *outs = filter(keys(__Exits), (: regexp($1, "^out") :));
    return sizeof(outs) == 1 ? outs[0] : 0;
}

varargs void set_exit (string dir, string dest, function before, function after, string reverse, string door, string key, int locked) {
    if (!stringp(dir)) error("Bad argument 1 to exit->set_exit");
    if (!stringp(dest)) error("Bad argument 2 to exit->set_exit");

    if (!mapp(__Exits[dir])) __Exits[dir] = ([ ]);
    __Exits[dir]["room"] = dest;
    if (functionp(before)) __Exits[dir]["before"] = before;
    if (functionp(after)) __Exits[dir]["after"] = after;
    if (stringp(reverse)) __Exits[dir]["reverse"] = reverse;
    if (stringp(door)) {
        __Exits[dir]["door"] = door;
        __Exits[dir]["open"] = 0;
        if (!undefinedp(key) && stringp(key)) __Exits[dir]["key"] = key;
        if (!undefinedp(locked) && intp(locked)) __Exits[dir]["locked"] = locked;
        else if (__Exits[dir]["key"]) {
            __Exits[dir]["locked"] = 1;
        }
    }
}
void set_exits (mapping exits) {
    __Exits = ([ ]);
    foreach (mixed dir, mixed dest in exits) {
        if (arrayp(dir)) {
            foreach (string real_dir in dir) {
                if (arrayp(dest)) set_exit(real_dir, dest...);
                else set_exit(real_dir, dest);
            }
        } else {
            if (stringp(dest)) set_exit(dir, dest);
            else if (arrayp(dest)) set_exit(dir, dest...);
        }
    }
}
void remove_exit (string dir) {
    if (!stringp(dir)) error("Bad argument 1 to exit->remove_exit");
    map_delete(__Exits, dir);
}
void set_hidden_exits (int n) {
    __HiddenExits = n;
}
int query_hidden_exits () {
    return __HiddenExits;
}

mixed handle_go (object ob, string method, string dir) {
    mapping exit;

    dir = format_exit_verbose(dir);

    if (!(exit = __Exits[dir])) {
        if (__Exits["enter " + dir] && !__Exits["out " + dir]) {
            dir = "enter " + dir;
            exit = __Exits[dir];
        } else if (__Exits["out " + dir] && !__Exits["enter " + dir]) {
            dir = "out " + dir;
            exit = __Exits[dir];
        }
    }

    if (!exit || environment(ob) != this_object()) {
        return 0;
    } else if (exit["before"] && !(evaluate(exit["before"], ob, dir))) {
        return 0;
    } else if (exit["room"]) {
        if ((regexp(exit["room"], "/virtual/")) || (regexp(exit["room"], "#[0-9]+") && find_object(exit["room"])) || (file_size(exit["room"]) > 0)) {
            if (exit["door"] && !exit["open"]) {
                message("action", "You bump into the " + exit["door"] + " blocking you from going " + dir + ".", ob);
                return 0;
            }
            ob->handle_go(exit["room"], method, dir, exit["reverse"]);
            ob->describe_environment();
            if (exit["after"]) {
                evaluate(exit["after"], ob, dir);
            }
            return 1;
        } else {
            message("action", "Something prevents you from going in that direction.", ob);
            return 0;
        }
    } else {
        return 0;
    }
}

/* ----- climbs ----- */

mapping query_climbs () {
    return copy(__Climbs);
}
string *query_climb_directions () {
    return keys(__Climbs);
}
string *query_climb_destinations () {
    return values(__Climbs);
}
string query_climb (string dir) {
    if (__Climbs[dir]) {
        return __Climbs[dir]["room"];
    }
    return 0;
}

varargs void set_climb (string dir, string dest, function before, function after, string reverse) {
    if (!stringp(dir)) error("Bad argument 1 to exit->set_climb");
    if (!stringp(dest)) error("Bad argument 2 to exit->set_climb");

    if (!mapp(__Climbs[dir])) __Climbs[dir] = ([ ]);
    __Climbs[dir]["room"] = dest;
    if (functionp(before)) __Climbs[dir]["before"] = before;
    if (functionp(after)) __Climbs[dir]["after"] = after;
    if (stringp(reverse)) __Climbs[dir]["reverse"] = reverse;
}
void set_climbs (mapping climbs) {
    __Climbs = ([ ]);
    foreach (mixed dir, mixed dest in climbs) {
        if (arrayp(dir)) {
            foreach (string real_dir in dir) {
                if (arrayp(dest)) set_climb(real_dir, dest...);
                else set_climb(real_dir, dest);
            }
        } else {
            if (stringp(dest)) set_climb(dir, dest);
            else if (arrayp(dest)) set_climb(dir, dest...);
        }
    }
}
void remove_climb (string dir) {
    if (!stringp(dir)) error("Bad argument 1 to exit->remove_climb");
    map_delete(__Climbs, dir);
}

mixed handle_climb (object ob, string method, string dir) {
    mapping exit;

    dir = format_exit_verbose(dir);

    if (!(exit = __Climbs[dir])) {
        if (__Climbs["enter " + dir] && !__Climbs["out " + dir]) {
            dir = "enter " + dir;
            exit = __Climbs[dir];
        } else if (__Climbs["out " + dir] && !__Climbs["enter " + dir]) {
            dir = "out " + dir;
            exit = __Climbs[dir];
        }
    }

    if (!exit || environment(ob) != this_object()) {
        return 0;
    } else if (exit["before"] && !(evaluate(exit["before"], ob, dir))) {
        return 0;
    } else if (exit["room"]) {
        if ((regexp(exit["room"], "#[0-9]+") && find_object(exit["room"])) || (file_size(exit["room"]) > 0)) {
            ob->handle_go(exit["room"], method, dir, exit["reverse"]);
            ob->describe_environment();
            if (exit["after"]) {
                evaluate(exit["after"], ob, dir);
            }
            return 1;
        } else {
            message("action", "Something prevents you from going in that direction.", ob);
            return 0;
        }
    } else {
        return 0;
    }
}

/* ----- doors ----- */

varargs string *query_doors (int directions) {
    mapping doors = map_mapping(filter_mapping(__Exits, (: $2["door"] :)), (: $2["door"] :));
    return (directions ? keys(doors) : ({ })) + values(doors);
}
string query_door_dir (string door) {
    mapping m = filter_mapping(__Exits, (: $2["door"] == $(door) :));
    if (sizeof(m)) {
        return keys(m)[0];
    }
    return 0;
}
string query_dir_door (string dir) {
    mapping doors = map_mapping(filter_mapping(__Exits, (: $2["door"] :)), (: $2["door"] :));
    if (member_array(dir, keys(doors)) > -1) {
        return __Exits[dir]["door"];
    }
    return 0;
}

int query_open (string str) {
    mapping doors = map_mapping(filter_mapping(__Exits, (: $2["door"] :)), (: $2["door"] :));

    if (member_array(str, values(doors)) > -1) {        // doors
        return __Exits[query_door_dir(str)]["open"];
    } else if (member_array(str, keys(doors)) > -1) {   // exits
        return __Exits[str]["open"];
    }
    return -1;
}
int query_locked (string str) {
    mapping doors = map_mapping(filter_mapping(__Exits, (: $2["door"] :)), (: $2["door"] :));

    if (member_array(str, values(doors)) > -1) {        // doors
        return __Exits[query_door_dir(str)]["locked"];
    } else if (member_array(str, keys(doors)) > -1) {   // exits
        return __Exits[str]["locked"];
    }
    return -1;
}

void set_open (string str, int open) {
    mapping doors = map_mapping(filter_mapping(__Exits, (: $2["door"] :)), (: $2["door"] :));
    string dir;

    if (member_array(str, values(doors)) > -1) {        // by door name
        dir = query_door_dir(str);
    } else if (member_array(str, keys(doors)) > -1) {   // by exit dir
        dir = str;
    } else {
        return;
    }
    if (!__Exits[dir]["locked"]) {
        __Exits[dir]["open"] = open;
    }
}
void set_locked (string str, int locked) {
    mapping doors = map_mapping(filter_mapping(__Exits, (: $2["door"] :)), (: $2["door"] :));

    if (member_array(str, values(doors)) > -1) {        // doors
        __Exits[query_door_dir(str)]["locked"] = locked;
    } else if (member_array(str, keys(doors)) > -1) {   // exits
        __Exits[str]["locked"] = locked;
    }
}

int handle_open (object ob, string str) {
    mapping doors = map_mapping(filter_mapping(__Exits, (: $2["door"] :)), (: $2["door"] :));
    string dir, door;

    if (member_array(str, values(doors)) > -1) {        // doors
        door = str;
        dir = query_door_dir(door);
    } else if (member_array(str = format_exit_verbose(str), keys(doors)) > -1) {   // exits
        dir = str;
        door = __Exits[dir]["door"];
    } else {
        return 0;
    }

    if (__Exits[dir]["locked"]) {
        message("action", "The " + door + " to the " + dir + " is locked.", ob);
        message("action", ob->query_cap_name() + " attempts to open the " + door + " to the " + dir + ".", environment(ob), ob);
    } else if (__Exits[dir]["open"]) {
        message("action", "The " + door + " to the " + dir + " is already open.", ob);
    } else {
        message("action", "You open the " + door + " to the " + dir + ".", ob);
        message("action", ob->query_cap_name() + " opens the " + door + " to the " + dir + ".", environment(ob), ob);
        __Exits[dir]["open"] = 1;
        __Exits[dir]["room"]->set_open(door, 1);
        message("action", "The " + door + " to the " + format_exit_reverse(dir) + " opens.", load_object(__Exits[dir]["room"]));
    }
    return 1;
}

int handle_close (object ob, string str) {
    mapping doors = map_mapping(filter_mapping(__Exits, (: $2["door"] :)), (: $2["door"] :));
    string dir, door;

    if (member_array(str, values(doors)) > -1) {        // doors
        door = str;
        dir = query_door_dir(door);
    } else if (member_array(str = format_exit_verbose(str), keys(doors)) > -1) {   // exits
        dir = str;
        door = __Exits[dir]["door"];
    } else {
        return 0;
    }

    if (!__Exits[dir]["open"]) {
        message("action", "The " + door + " to the " + dir + " is already closed.", ob);
    } else {
        message("action", "You close the " + door + " to the " + dir + ".", ob);
        message("action", ob->query_cap_name() + " closes the " + door + " to the " + dir + ".", environment(ob), ob);
        __Exits[dir]["open"] = 0;
        __Exits[dir]["room"]->set_open(door, 0);
        message("action", "The " + door + " to the " + format_exit_reverse(dir) + " closes.", load_object(__Exits[dir]["room"]));
    }
    return 1;
}

int handle_lock (object ob, string str, object key) {
    mapping doors = map_mapping(filter_mapping(__Exits, (: $2["door"] :)), (: $2["door"] :));
    string dir, door;

    if (member_array(str, values(doors)) > -1) {        // doors
        door = str;
        dir = query_door_dir(door);
    } else if (member_array(str = format_exit_verbose(str), keys(doors)) > -1) {   // exits
        dir = str;
        door = __Exits[dir]["door"];
    } else {
        return 0;
    }

    if (!__Exits[dir]["key"]) {
        message("action", "The " + door + " to the " + dir + " does not have a lock.", ob);
    } else if (__Exits[dir]["key"]["open"]) {
        message("action", "The " + door + " to the " + dir + " cannot be locked while open.", ob);
    } else if (__Exits[dir]["locked"]) {
        message("action", "The " + door + " to the " + dir + " is already locked.", ob);
    } else if (key->query_name() == __Exits[dir]["key"]) {
        message("action", "You lock the " + door + " to the " + dir + " with " + key->query_short() + ".", ob);
        message("action", ob->query_cap_name() + " locks the " + door + " to the " + dir + " with " + key->query_short() + ".", environment(ob), ob);
        __Exits[dir]["locked"] = 1;
        __Exits[dir]["room"]->set_locked(door, 1);
        message("action", "The " + door + " to the " + format_exit_reverse(dir) + " clicks locked.", load_object(__Exits[dir]["room"]));
    }
    return 1;
}

int handle_unlock (object ob, string str, object key) {
    mapping doors = map_mapping(filter_mapping(__Exits, (: $2["door"] :)), (: $2["door"] :));
    string dir, door;

    if (member_array(str, values(doors)) > -1) {        // doors
        door = str;
        dir = query_door_dir(door);
    } else if (member_array(str = format_exit_verbose(str), keys(doors)) > -1) {   // exits
        dir = str;
        door = __Exits[dir]["door"];
    } else {
        return 0;
    }

    if (!__Exits[dir]["key"]) {
        message("action", "The " + door + " to the " + dir + " does not have a lock.", ob);
    } else if (__Exits[dir]["open"]) {
        message("action", "The " + door + " to the " + dir + " cannot be unlocked while open.", ob);
    } else if (!__Exits[dir]["locked"]) {
        message("action", "The " + door + " to the " + dir + " is already unlocked.", ob);
    } else if (key->query_name() == __Exits[dir]["key"]) {
        message("action", "You unlock the " + door + " to the " + dir + " with " + key->query_short() + ".", ob);
        message("action", ob->query_cap_name() + " unlocks the " + door + " to the " + dir + " with " + key->query_short() + ".", environment(ob), ob);
        __Exits[dir]["locked"] = 0;
        __Exits[dir]["room"]->set_locked(door, 0);
        message("action", "The " + door + " to the " + format_exit_reverse(dir) + " clicks unlocked.", load_object(__Exits[dir]["room"]));
    }
    return 1;
}