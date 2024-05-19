mapping __Cooldown = ([ ]);

private void initialize_cooldowns () {
    if (!mapp(__Cooldown)) {
        __Cooldown = ([ ]);
    }
}

/*
    type can be "heart_beat" or "timed"
*/
varargs void set_cooldown (string name, mixed value, string type) {
    initialize_cooldowns();
    if (!stringp(name)) {
        error("Bad argument 1 to cooldown->set_cooldown");
    }
    if (!stringp(type)) {
        type = "heart_beat";
    }
    if (type == "heart_beat" && !intp(value)) {
        error("Bad argument 2 to cooldown->set_cooldown");
    } else if (type == "timed" && !floatp(value)) {
        error("Bad argument 2 to cooldown->set_cooldown");
    } else if (type != "heart_beat" && type != "timed") {
        error("Bad argument 3 to cooldown->set_cooldown");
    }
    if (value > -1) {
        __Cooldown[name] = ([
            "type": type,
            "value": value,
        ]);

        if (type == "timed") {
            call_out_walltime("cooldown_timed_expire", value, name);
        }
    }
}
int query_cooldown (string name) {
    initialize_cooldowns();
    return __Cooldown[name] && __Cooldown[name]["value"] || 0;
}
mapping query_cooldowns () {
    initialize_cooldowns();
    return __Cooldown;
}

protected void cooldown_timed_expire (string name) {
    map_delete(__Cooldown, name);
}

/* ----- applies ----- */

void heart_beat () {
    foreach (string name, mapping data in __Cooldown || ([ ])) {
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