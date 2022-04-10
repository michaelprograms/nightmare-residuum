inherit STD_OBJECT;
inherit M_CLEAN;

int is_item () { return 1; }

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
    return 1;
}
mixed direct_get_obj_from_obj (mixed args...) {
    object env = environment(environment());
    if (env != environment(previous_object())) {
        if (env == previous_object()) {
            return 0;
        } else {
            return "You can't get what isn't here.";
        }
    }
    return 1;
}

/* ----- object applies ----- */

void reset () {
    ::reset();

    foreach (string file in inherit_list(this_object()) - ({ STD_ITEM })) {
        string name = split_path(file)[1][0..<3];
        call_other(this_object(), "reset_" + name);
    }
}