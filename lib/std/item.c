inherit STD_OBJECT;
inherit M_CLEAN;

mixed direct_look_at_obj () {
    return environment() == environment(previous_object());
}
mixed direct_look_obj () {
    return direct_look_at_obj();
}
mixed direct_drop_obj (object ob, string str) {
    if (environment() != this_character()) {
        return "You can't drop what you don't have.";
    }
    return 1;
}
mixed direct_get_obj (object ob, string str) {
    object env = environment();
    if (env != environment(this_character())) {
        if (env == this_character()) {
            return 0;
        } else {
            return "You can't get what you isn't here.";
        }
    }
    return 1;
}
