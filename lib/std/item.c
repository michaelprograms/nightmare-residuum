inherit STD_OBJECT;

mixed direct_look_at_obj () {
    return environment() == environment(previous_object());
}
mixed direct_look_obj () {
    return direct_look_at_obj();
}