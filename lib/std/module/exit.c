nosave private mapping __Exits = ([]);
nosave private mapping __Door = ([]); // @TODO

mapping query_exits () { return copy(__Exits); }
string *query_exit_directions () { return keys(__Exits); }
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
    string *enters = filter(keys(__Exits), (: strsrch($1, "enter") > -1 :));
    return sizeof(enters) == 1 ? enters[0] : 0;
}
string query_default_out () {
    string *outs = filter(keys(__Exits), (: strsrch($1, "out") > -1 :));
    return sizeof(outs) == 1 ? outs[0] : 0;
}

varargs void set_exit (string dir, string dest, function before, function after) {
    if (!stringp(dir)) raise_error("Bad argument 1 to exit->set_exit");
    if (!stringp(dest)) raise_error("Bad argument 2 to exit->set_exit");

    if (!mappingp(__Exits[dir])) __Exits[dir] = ([]);
    __Exits[dir]["room"] = dest;
    if (closurep(before)) __Exits[dir]["before"] = before;
    if (closurep(after)) __Exits[dir]["after"] = after;
}
void set_exits (mapping exits) {
    __Exits = ([]);
    foreach (mixed dir, mixed dest in exits) {
        if (pointerp(dir)) {
            foreach (string real_dir in dir) {
                if (pointerp(dest)) set_exit(real_dir, dest...);
                else set_exit(real_dir, dest);
            }
        } else {
            if (stringp(dest)) set_exit(dir, dest);
            else if (pointerp(dest)) set_exit(dir, dest...);
        }
    }
}
void remove_exit (string dir) {
    if (!stringp(dir)) raise_error("Bad argument 1 to exit->remove_exit");
    map_delete(__Exits, dir);
}

// -----------------------------------------------------------------------------

mixed handle_go (object ob, string method, string dir) {
    mapping exit;

    // if(query_verb() == "go" && interactive(ob)) {
    //     // @TODO check standng/sitting?
    // }

    dir = format_exit_verbose(dir);

    if (!(exit = __Exits[dir])) {
        if (__Exits["enter " + dir] && !__Exits["out " + dir]) exit = __Exits["enter " + dir];
        else if (__Exits["out " + dir] && !__Exits["enter " + dir]) exit = __Exits["out " + dir];
    }

    if (!exit || environment(ob) != this_object()) {
        return 0;
    } else if (exit["before"] && !(funcall(exit["before"], ob, dir))) {
        return 0;
    } else if (exit["room"]) {
        if ((sizeof(regexp(exit["room"], "#[0-9]+")) && find_object(exit["room"])) || (file_size(exit["room"]) > 0)) {
            ob->handle_go(exit["room"], method, dir);
            ob->describe_environment();
            if (exit["after"]) {
                funcall(exit["after"], ob, dir);
            }
            return 1;
        } else {
            message("action", "Something prevents you from going in that direction.\n", ob);
            return 0;
        }
    } else {
        return 0;
    }
}