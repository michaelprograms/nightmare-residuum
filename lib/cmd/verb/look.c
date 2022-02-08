inherit "/std/verb.c";

void create () {
    verb::create();
    add_rules(({ "", "OBJ", "at OBJ", "LIV", "at LIV", "STR", "at STR", }));
}

/* -------------------- */

mixed can_look () {
    if (!environment(this_character())) return "There is nothing.";
    else return 1;
}
mixed do_look () {
    this_character()->describe_environment();
    return 1;
}

/* -------------------- */

mixed can_look_at_str (string str, string verb) {
    return 1;
}
mixed can_look_str (string str, string verb) {
    return can_look_at_str(str, verb);
}
void do_look_at_str (string str) {
    environment(this_character())->do_look_at_str(str);
}
void do_look_str (string str) {
    do_look_at_str(str);
}

/* -------------------- */

mixed can_look_at_obj (string str, string verb) {
    return 1;
}
mixed can_look_obj (string str, string verb) {
    return can_look_at_obj(str, verb);
}
varargs mixed do_look_at_obj (object ob, mixed arg) {
    string str = ob->query_long();

    if (sizeof(str) && str[<1] != '\n') str += "\n";
    write(str);
    return 1;
}
varargs mixed do_look_obj (object ob, mixed *args...) {
    return do_look_at_obj(ob, args...);
}

/* -------------------- */

mixed can_look_at_liv (string str, string verb) {
    return 1;
}
mixed can_look_liv (string str, string verb) {
    return can_look_at_liv(str, verb);
}
varargs mixed do_look_at_liv (object ob, mixed arg) {
    string str = ob->query_long();

    if (sizeof(str) && str[<1] != '\n') str += "\n";
    write("You look over " + ob->query_name() + " the " + ob->query_species() + "...\n");
    write(str + "\n");
    return 1;
}
varargs mixed do_look_liv (object ob, mixed *args...) {
    return do_look_at_liv(ob, args...);
}