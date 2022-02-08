nosave private mapping __Looks = ([]);

mixed query_look (string look) {
    if (!stringp(look)) error("Bad argument 1 to looks->query_look");
    return __Looks[look] || __Looks[look + "s"];
}
string *query_looks () {
    return keys(__Looks);
}
varargs void set_look (string look, mixed desc) {
    if (!stringp(look)) error("Bad argument 1 to looks->set_look");
    if (!stringp(desc) && !functionp(desc)) error("Bad argument 2 to looks->set_look");

    if (!mapp(__Looks)) __Looks = ([]);
    __Looks[look] = desc;
}
void set_looks (mapping looks) {
    __Looks = ([]);
    foreach (mixed look, mixed desc in looks) {
        if (arrayp(look)) {
            foreach (string real_look in look) {
                set_look(real_look, desc);
            }
        } else {
            if (stringp(desc)) set_look(look, desc);
            else if (arrayp(desc)) set_look(look, desc);
        }
    }
}
void remove_look (string look) {
    if (!stringp(look)) error("Bad argument 1 to looks->remove_look");
    map_delete(__Looks, look);
}

/* -------------------- */

mixed direct_look_at_str () {
    return environment() == environment(previous_object());
}
mixed direct_look_str () {
    return direct_look_at_str();
}

void do_look_at_str (string look) {
    mixed desc;

    if (!look) return;
    desc = query_look(look);
    if (functionp(desc)) {
        desc = evaluate(desc, this_character());
    }
    message("room_look", desc + "\n", this_character());
}
void do_look_str (string look) {
    do_look_at_str(look);
}