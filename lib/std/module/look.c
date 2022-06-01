nosave private mapping __Looks = ([]);

mixed query_look (string look) {
    if (!stringp(look)) return 0;
    return __Looks[look] || __Looks[look + "s"];
}
string *query_looks () {
    return sort_array(keys(__Looks), 1);
}
varargs void set_look (string look, mixed desc) {
    if (!stringp(look)) raise_error("Bad argument 1 to look->set_look");
    if (!stringp(desc) && !closurep(desc)) raise_error("Bad argument 2 to look->set_look");

    if (!mappingp(__Looks)) __Looks = ([]);
    __Looks[look] = desc;
}
void set_looks (mapping looks) {
    if (!mappingp(looks)) raise_error("Bad argument 1 to look->set_looks");

    __Looks = ([]);
    foreach (mixed look, mixed desc in looks) {
        if (pointerp(look)) {
            foreach (string real_look in look) {
                set_look(real_look, desc);
            }
        } else {
            set_look(look, desc);
        }
    }
}
void remove_look (string look) {
    if (!stringp(look)) raise_error("Bad argument 1 to look->remove_look");
    map_delete(__Looks, look);
}

mixed handle_look (string look) {
    mixed desc;

    if (!stringp(look)) raise_error("Bad argument 1 to look->handle_look");
    desc = query_look(look);
    if (closurep(desc)) {
        desc = funcall(desc, this_character());
    }
    if (!desc) return 0;
    return desc;
}
