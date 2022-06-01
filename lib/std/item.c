inherit STD_OBJECT;
inherit M_CLEAN;

nosave private mixed __NoGet;

int is_item () { return 1; }

mixed query_no_get () {
    return __NoGet;
}
void set_no_get (mixed condition) {
    if (!stringp(condition) && !intp(condition) && !closurep(condition)) {
        raise_error("Bad argument 1 to item->set_no_get");
    }
    __NoGet = condition;
}

/* ----- parser applies ----- */

mixed direct_look_at_obj (mixed args...) {
    return environment(this_object()) && (environment(this_object()) == environment(previous_object()) || environment(this_object()) == previous_object());
}
mixed direct_look_obj (mixed args...) {
    return direct_look_at_obj(args);
}
mixed direct_drop_obj (mixed args...) {
    return (environment(this_object()) == previous_object());
}
mixed direct_give_obj_to_liv (mixed args...) {
    return (environment(this_object()) == previous_object());
}
mixed direct_get_obj (mixed args...) {
    object env = environment(this_object());
    if (env != environment(previous_object())) {
        if (env == previous_object()) {
            return 0;
        } else {
            return "You can't get what isn't here.";
        }
    }
    if (__NoGet) {
        if (closurep(__NoGet)) {
            return funcall(__NoGet, previous_object());
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
    if (environment(this_object())) {
        env = environment(environment(this_object()));
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