private mapping __Cooldown = ([ ]);

varargs void set_cooldown (string name, int value, string type) {
    if (!mapp(__Cooldown)) {
        __Cooldown = ([ ]);
    }
    if (!stringp(name)) {
        error("Bad argument 1 to cooldown->set_cooldown");
    }
    if (!intp(value)) {
        error("Bad argument 2 to cooldown->set_cooldown");
    }
    if (!stringp(type)) {
        type = "heart_beat";
    }
    if (value > -1) {
        __Cooldown[name] = ([
            "type": type,
            "value": value,
        ]);
    }
}
int query_cooldown (string name) {
    if (!mapp(__Cooldown)) {
        __Cooldown = ([ ]);
    }
    return __Cooldown[name] || 0;
}
mapping query_cooldowns () {
    if (!mapp(__Cooldown)) {
        __Cooldown = ([ ]);
    }
    return __Cooldown;
}

/* ----- applies ----- */

void heart_beat () {
    foreach (string name, mapping data in __Cooldown) {
        if (data["type"] == "heart_beat") {
            if (data["value"] > 0) {
                data["value"] --;
                if (data["value"] == 0) {
                    map_delete(__Cooldown, name);
                }
            }
        }
    }
}