nosave private mapping __Looks = ([ ]);

mixed query_look (string look) {
    if (!stringp(look)) {
        return 0;
    }
    return __Looks[look] || __Looks[look + "s"];
}
string *query_looks () {
    return sort_array(keys(__Looks), 1);
}
varargs void set_look (string look, mixed desc) {
    if (!stringp(look)) {
        error("Bad argument 1 to look->set_look");
    }
    if (!stringp(desc) && !functionp(desc)) {
        error("Bad argument 2 to look->set_look");
    }

    if (!mapp(__Looks)) {
        __Looks = ([ ]);
    }
    __Looks[look] = desc;
}
void set_looks (mapping looks) {
    if (!mapp(looks)) {
        error("Bad argument 1 to look->set_looks");
    }

    __Looks = ([ ]);
    foreach (mixed look, mixed desc in looks) {
        if (arrayp(look)) {
            foreach (string real_look in look) {
                set_look(real_look, desc);
            }
        } else {
            set_look(look, desc);
        }
    }
}
void remove_look (string look) {
    if (!stringp(look)) {
        error("Bad argument 1 to look->remove_look");
    }
    map_delete(__Looks, look);
}

mixed handle_look (string look) {
    mixed desc;

    if (!stringp(look)) {
        error("Bad argument 1 to look->handle_look");
    }
    desc = query_look(look);
    if (functionp(desc)) {
        desc = evaluate(desc, this_character());
    }
    if (!desc) {
        return 0;
    }
    return desc;
}
