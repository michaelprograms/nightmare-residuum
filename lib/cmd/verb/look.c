inherit STD_VERB;

void create () {
    verb::create();
    add_rules(({ "", "STR", "at STR", "OBJ", "at OBJ", "LIV", "at LIV", }));
}

/* ----- rule: "" ----- */

mixed can_look () {
    if (!environment(previous_object())) return "There is nothing.";
    else return 1;
}
mixed do_look () {
    previous_object()->describe_environment();
    return 1;
}

/* ----- rule: "STR" and "at STR" ----- */

mixed can_look_at_str (string str, string verb) {
    return 1;
}
mixed can_look_str (string str, string verb) {
    return can_look_at_str(str, verb);
}
void do_look_at_str (string str) {
    object po = previous_object();
    string desc = environment(po)->handle_look(str);

    if (!desc) {
        message("verb", "You do not see that here.\n", po);
    } else {
        message("verb", desc + "\n", po);
        message("verb", po->query_cap_name() + " looks over the " + str + ".\n", environment(po), po);
    }
}
void do_look_str (string str) {
    do_look_at_str(str);
}

/* ----- rule: "OBJ" and "at OBJ" ----- */

mixed can_look_at_obj (string str, string verb) {
    return 1;
}
mixed can_look_obj (string str, string verb) {
    return can_look_at_obj(str, verb);
}
varargs mixed do_look_at_obj (object ob, mixed arg) {
    object po = previous_object();
    string long = ob->query_long();

    if (sizeof(long) && long[<1] != '\n') long += "\n";

    message("verb", "You look over " + ob->query_name() + "...\n" + long, po);
    message("verb", po->query_cap_name() + " looks over " + ob->query_name() + ".\n", environment(ob), po);

    return 1;
}
varargs mixed do_look_obj (object ob, mixed *args...) {
    return do_look_at_obj(ob, args...);
}

/* ----- rule: "LIV" and "at LIV" ----- */

mixed can_look_at_liv (string str, string verb) {
    return 1;
}
mixed can_look_liv (string str, string verb) {
    return can_look_at_liv(str, verb);
}
mixed do_look_at_liv (object ob, mixed arg...) {
    object po = previous_object();
    string str;
    object *wielded, *worn;

    str = (ob->query_gender() == "neither" || ob->query_gender() == "none" ? "" : ob->query_gender() + " ");
    message("verb", "You look over " + ob->query_cap_name() + " the " + str + ob->query_species() + "...\n", po);
    message("verb", po->query_cap_name() + " looks you over.\n", ob);
    message("verb", po->query_cap_name() + " looks over " + ob->query_cap_name() + ".\n", environment(ob), ({ po, ob }));

    if (str = ob->query_long()) {
        if (sizeof(str) && str[<1] != '\n') str += "\n";
        message("verb", str + "\n", po);
    }
    if (po->query_immortal()) {
        message("verb", "Level: "+ob->query_level()+"\n", po);
        message("verb", "Vitals: "+ob->query_hp()+"/"+ob->query_max_hp()+" "+ob->query_sp()+"/"+ob->query_max_sp()+" "+ob->query_mp()+"/"+ob->query_max_mp()+"\n", po);
    }

    if (sizeof(wielded = ob->query_wielded_weapons())) {
        message("verb", capitalize(subjective(ob)) + " is wielding:\n", po);
        foreach (string weapon in wielded) {
            message("verb", "  " + weapon->query_short() + "\n", po);
        }
    }
    if (sizeof(worn = ob->query_all_armor())) {
        message("verb", capitalize(subjective(ob)) + " is wearing:\n", po);
        foreach (string armor in worn) {
            message("verb", "  " + armor->query_short() + "\n", po);
        }
    }
    return 1;
}
mixed do_look_liv (object ob, mixed args...) {
    return do_look_at_liv(ob, args...);
}