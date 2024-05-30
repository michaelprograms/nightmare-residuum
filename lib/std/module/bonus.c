// Inherited by armor or weapons to keep track of bonuses

nosave mapping __Bonuses = ([ ]);

private void initialize_bonuses () {
    if (!mapp(__Bonuses)) {
        __Bonuses = ([ ]);
    }
}

/* ----- querying ----- */

mixed query_bonus (string key) {
    initialize_bonuses();
    return __Bonuses[key];
}
mapping query_bonuses () {
    initialize_bonuses();
    return __Bonuses;
}

/* ----- setting ----- */

mixed set_bonus (string key, mixed value) {
    initialize_bonuses();
    return __Bonuses[key] = value;
}
mapping set_bonuses (mapping bonuses) {
    initialize_bonuses();
    foreach (string key, int value in bonuses) {
        set_bonus(key, value);
    }
    return __Bonuses;
}

/* ----- applying ----- */

void apply_bonus (object target) {
    foreach (string key, int value in __Bonuses) {
        if (member_array(key, ({ "strength", "perception", "endurance", "charisma", "intelligence", "agility", "luck", })) > -1) {
            target->add_stat_bonus(key, value);
        }
    }
}
void remove_bonus (object target) {
    foreach (string key, int value in __Bonuses) {
        if (member_array(key, ({ "strength", "perception", "endurance", "charisma", "intelligence", "agility", "luck", })) > -1) {
            target->add_stat_bonus(key, -value);
        }
    }
}