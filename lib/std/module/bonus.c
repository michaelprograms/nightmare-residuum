// Inherited by armor or weapons to keep track of bonuses

nosave private mapping __Bonuses = ([ ]);

mixed query_bonus (string key) {
    if (!stringp(key) || key == "") raise_error("Bad argument 1 to bonus->query_bonus");
    if (!mappingp(__Bonuses)) __Bonuses = ([ ]);
    return __Bonuses[key];
}
mapping query_bonuses () {
    if (!mappingp(__Bonuses)) __Bonuses = ([ ]);
    return __Bonuses;
}
mixed set_bonus (string key, mixed value) {
    if (!stringp(key) || key == "") raise_error("Bad argument 1 to bonus->set_bonus");
    if (!intp(value)) raise_error("Bad argument 2 to bonus->set_bonus");
    if (!mappingp(__Bonuses)) __Bonuses = ([ ]);
    return __Bonuses[key] = value;
}
mapping set_bonuses (mapping bonuses) {
    if (!mappingp(bonuses) || !sizeof(bonuses)) raise_error("Bad argument 1 to bonus->set_bonuses");
    if (!mappingp(__Bonuses)) __Bonuses = ([ ]);
    foreach (string key, int value in bonuses) {
        set_bonus(key, value);
    }
    return __Bonuses;
}

void add_bonuses (object target) {
    foreach (string key, int value in __Bonuses) {
        if (member(key, ({ "strength", "perception", "endurance", "charisma", "intelligence", "agility", "luck", })) > -1) {
            target->add_stat_bonus(key, value);
        }
    }
}

void remove_bonuses (object target) {
    foreach (string key, int value in __Bonuses) {
        if (member(key, ({ "strength", "perception", "endurance", "charisma", "intelligence", "agility", "luck", })) > -1) {
            target->add_stat_bonus(key, -value);
        }
    }
}