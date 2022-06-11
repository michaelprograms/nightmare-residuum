inherit STD_OBJECT;
inherit M_CLEAN;
inherit M_AUTOLOAD;

nosave private mixed __NoGet;
nosave private int __Value;

/* -----  ----- */

int is_item () { return 1; }

/* ----- value ----- */

int query_value () {
    return __Value;
}
void set_value (int v) {
    if (!intp(v) || v < 0) error("Bad argument 1 to item->set_value");
    __Value = v;
}

/* ----- no get ----- */

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

mixed direct_look_at_obj (mixed args...) {
    return environment() && (environment() == environment(previous_object()) || environment() == previous_object());
}
mixed direct_look_obj (mixed args...) {
    return direct_look_at_obj(args);
}
mixed direct_drop_obj (mixed args...) {
    return (environment() == previous_object());
}
mixed direct_give_obj_to_liv (mixed args...) {
    return (environment() == previous_object());
}
mixed direct_get_obj (mixed args...) {
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
    if (!sizeof(args) || !args[0]) return 0;
    if (environment(args[0]) != previous_object()) {
        return "You can't put what you don't have.";
    }
    return 1;
}
mixed direct_sell_obj (mixed args...) {
    if (!sizeof(args) || !args[0]) return 0;
    if (environment(args[0]) != previous_object()) {
        return "You can't sell what you don't have.";
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