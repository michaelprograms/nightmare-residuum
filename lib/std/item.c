inherit STD_OBJECT;
inherit M_CLEAN;

nosave private mixed __NoGet;

int is_item () { return 1; }

mixed query_no_get () {
    return __NoGet;
}
void set_no_get (mixed condition) {
    if (!stringp(condition) && !intp(condition) && !functionp(condition)) {
        error("Bad argument 1 to item->set_no_get");
    }
    __NoGet = condition;
}

/* ----- parser applies ----- */

mixed direct_look_at_obj () {
    return environment() && (environment() == environment(previous_object()) || environment() == previous_object());
}
mixed direct_look_obj () {
    return direct_look_at_obj();
}
mixed direct_drop_obj (object ob, string str) {
    return (environment() == previous_object());
}
varargs mixed direct_give_obj_to_liv (object ob, mixed liv) {
    return (environment() == previous_object());
}
mixed direct_get_obj (object ob, string str) {
    object env = environment();
    if (env != environment(previous_object())) {
        if (env == previous_object()) {
            return 0;
        } else {
            return "You can't get what isn't here.";
        }
    }
    if (__NoGet) {
        if (functionp(__NoGet)) {
            return evaluate(__NoGet, previous_object());
        } else if (stringp(__NoGet)) {
            return __NoGet;
        } else if (intp(__NoGet)) {
            return "You can't get the "+ query_name() + ".";
        }
    }
    return 1;
}
mixed direct_get_obj_from_obj (mixed args...) {
    object env;
    if (environment()) {
        env = environment(environment());
    }
    if (env != environment(previous_object())) {
        if (env == previous_object()) {
            return 0;
        } else {
            return "You can't get what isn't here.";
        }
    }
    return 1;
}
mixed direct_put_obj_in_obj (mixed args...) {
    if (!args || !args[0]) return 0;
    if (environment(args[0]) != previous_object()) {
        return "You can't put what you don't have.";
    }
    return 1;
}

/* ----- object applies ----- */

void reset () {
    ::reset();

    foreach (string file in inherit_list() - ({ STD_ITEM })) {
        string name = split_path(file)[1][0..<3];
        call_other(this_object(), "reset_" + name);
    }
}