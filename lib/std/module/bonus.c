// Inherited by armor or weapons to keep track of bonuses

nosave private mapping __Bonuses = ([ ]);

mixed query_bonus (string key) {
    if (!stringp(key) || key == "") error("Bad argument 1 to bonus->query_bonus");
    if (!mapp(__Bonuses)) __Bonuses = ([ ]);
    return __Bonuses[key];
}
mapping query_bonuses () {
    if (!mapp(__Bonuses)) __Bonuses = ([ ]);
    return __Bonuses;
}
mixed set_bonus (string key, mixed value) {
    if (!stringp(key) || key == "") error("Bad argument 1 to bonus->set_bonus");
    if (!intp(value)) error("Bad argument 2 to bonus->set_bonus");
    if (!mapp(__Bonuses)) __Bonuses = ([ ]);
    return __Bonuses[key] = value;
}
mapping set_bonuses (mapping bonuses) {
    if (!mapp(bonuses) || !sizeof(bonuses)) error("Bad argument 1 to bonus->set_bonuses");
    if (!mapp(__Bonuses)) __Bonuses = ([ ]);
    foreach (string key, int value in bonuses) {
        set_bonus(key, value);
    }
    return __Bonuses;
}

void add_bonuses () {
    foreach (string key, int value in __Bonuses) {
        if (member_array(key, ({ "strength", "perception", "endurance", "charisma", "intelligence", "agility", "luck", })) > -1) {
            this_object()->add_stat_bonus(key, value);
        }
    }
}

void remove_bonuses () {
    foreach (string key, int value in __Bonuses) {
        if (member_array(key, ({ "strength", "perception", "endurance", "charisma", "intelligence", "agility", "luck", })) > -1) {
            this_object()->add_stat_bonus(key, -value);
        }
    }
}