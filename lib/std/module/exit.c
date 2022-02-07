nosave private mapping __Exits = ([]);
nosave private mapping __Door = ([]);

mapping query_exits () { return copy(__Exits); }
string *query_exit_directions () { return keys(__Exits); }
string *query_exit_dirs () { // shorthand directions
    string *exits = keys(__Exits);
    for (int i = 0; i < sizeof(exits); i ++) {
        switch (exits[i]) {
        case "north": exits[i] = "n"; break;
        case "northeast": exits[i] = "ne"; break;
        case "east": exits[i] = "e"; break;
        case "southeast": exits[i] = "se"; break;
        case "south": exits[i] = "s"; break;
        case "southwest": exits[i] = "sw"; break;
        case "west": exits[i] = "w"; break;
        case "northwest": exits[i] = "nw"; break;
        case "up": exits[i] = "u"; break;
        case "down": exits[i] = "d"; break;
        case "enter": exits[i] = "en"; break;
        }
    }
    return exits;
}
string *query_exit_destinations () {
    return values(__Exits);
}
string query_exit (string dir) {
    return __Exits[dir] && __Exits[dir]["room"];
}

varargs void set_exit (string dir, string dest, function before, function after) {
    if (!stringp(dir)) error("Bad argument 1 to exit->set_exit");
    if (!stringp(dest)) error("Bad argument 2 to exit->set_exit");

    if (!mapp(__Exits[dir])) __Exits[dir] = ([]);
    __Exits[dir]["room"] = dest;
    if (functionp(before)) __Exits[dir]["before"] = before;
    if (functionp(after)) __Exits[dir]["after"] = after;
}
void set_exits (mapping exits) {
    __Exits = ([]);
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

// -----------------------------------------------------------------------------

mixed handle_go (object ob, string dir) {
    // if(query_verb() == "go" && interactive(ob)) {
    //     // @TODO check standng/sitting?
    // }
    if (!__Exits[dir] || environment(ob) != this_object()) {
        return 0;
    } else if (__Exits[dir]["before"] && !(evaluate(__Exits[dir]["before"], ob, dir))) {
        return 0;
    } else if (__Exits[dir]["room"]) {
        if ((regexp(__Exits[dir]["room"], "#[0-9]+") && find_object(__Exits[dir]["room"])) || (file_size(__Exits[dir]["room"]) > 0)) {
            ob->handle_move(__Exits[dir]["room"]);
            ob->describe_environment();
            if (__Exits[dir]["after"]) {
                evaluate(__Exits[dir]["after"], ob, dir);
            }
            return 1;
        } else {
            return 0;
        }
    } else {
        return 0;
    }
}