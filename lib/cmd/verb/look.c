inherit STD_VERB;

void create () {
    verb::create();
    add_rules(({ "", "OBJ", "at OBJ", "LIV", "at LIV", "STR", "at STR", }));
}

/* -------------------- */

mixed can_look () {
    if (!environment(previous_object())) return "There is nothing.";
    else return 1;
}
mixed do_look () {
    previous_object()->describe_environment();
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
    environment(previous_object())->do_look_at_str(str);
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
    string str;
    object *wielded, *worn;

    str = (ob->query_gender() == "neither" || ob->query_gender() == "none" ? "" : ob->query_gender() + " ");
    write("You look over " + ob->query_cap_name() + " the " + str + ob->query_species() + "...\n");
    message("action", previous_object()->query_cap_name() + " looks you over.\n", ob);
    message("action", previous_object()->query_cap_name() + " looks over " + ob->query_cap_name() + ".\n", environment(ob), ({ previous_object(), ob }));

    if (str = ob->query_long()) {
        if (sizeof(str) && str[<1] != '\n') str += "\n";
        write(str + "\n");
    }
    if (previous_object()->query_immortal()) {
        write("Level: "+ob->query_level()+"\n");
        write("Vitals: "+ob->query_hp()+"/"+ob->query_max_hp()+" "+ob->query_sp()+"/"+ob->query_max_sp()+" "+ob->query_mp()+"/"+ob->query_max_mp()+"\n");
    }

    if (sizeof(wielded = ob->query_wielded_weapons())) {
        write(capitalize(subjective(ob)) + " is wielding:\n");
        foreach (string weapon in wielded) {
            write("  " + weapon->query_short() + "\n");
        }
    }
    if (sizeof(worn = ob->query_worn_armor())) {
        write(capitalize(subjective(ob)) + " is wearing:\n");
        foreach (string armor in worn) {
            write("  " + armor->query_short() + "\n");
        }
    }
    return 1;
}
varargs mixed do_look_liv (object ob, mixed *args...) {
    return do_look_at_liv(ob, args...);
}