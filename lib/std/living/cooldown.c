private mapping __Cooldown = ([ ]);

// Disable prevents actions, combat hits, and movement until expired
void set_cooldown (string name, int value) {
    if (!mapp(__Cooldown)) {
        __Cooldown = ([ ]);
    }
    if (!stringp(name)) {
        error("Bad argument 1 to cooldown->set_cooldown");
    }
    if (!intp(value)) {
        error("Bad argument 2 to cooldown->set_cooldown");
    }
    if (value > -1) {
        __Cooldown[name] = value;
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
    foreach (string name, int value in __Cooldown) {
        if (__Cooldown[name] > 0) {
            __Cooldown[name] --;
            if (__Cooldown[name] == 0) {
                map_delete(__Cooldown, name);
            }
        }
    }
}