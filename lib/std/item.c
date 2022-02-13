inherit STD_OBJECT;
inherit M_CLEAN;

mixed direct_look_at_obj () {
    return environment() && environment() == environment(previous_object());
}
mixed direct_look_obj () {
    return direct_look_at_obj();
}
mixed direct_drop_obj (object ob, string str) {
    return (environment() == this_character());
}
mixed direct_give_obj_to_liv (object ob, mixed liv, string str, string str2) {
    return (environment() == this_character());
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
