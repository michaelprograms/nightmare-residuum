// @this_object /std/living.c

mapping __Stat = ([
    "strength": 0,
    "perception": 0,
    "endurance": 0,
    "charisma": 0,
    "intelligence": 0,
    "agility": 0,
    "luck": 0,
]);
nosave mapping __StatBonus = ([
    "strength": 0,
    "perception": 0,
    "endurance": 0,
    "charisma": 0,
    "intelligence": 0,
    "agility": 0,
    "luck": 0,
]);

private void initialize_stats () {
    if (!mapp(__Stat)) {
        __Stat = ([
            "strength": 0,
            "perception": 0,
            "endurance": 0,
            "charisma": 0,
            "intelligence": 0,
            "agility": 0,
            "luck": 0,
        ]);
    }
    if (!mapp(__StatBonus)) {
        __StatBonus = ([
            "strength": 0,
            "perception": 0,
            "endurance": 0,
            "charisma": 0,
            "intelligence": 0,
            "agility": 0,
            "luck": 0,
        ]);
    }
}

int query_stat_base (string stat) {
    initialize_stats();
    return __Stat[stat];
}
int query_stat_bonus (string stat) {
    initialize_stats();
    return __StatBonus[stat];
}
int query_stat (string stat) {
    initialize_stats();
    return __Stat[stat] + query_stat_bonus(stat);
}

void set_stat (string stat, int n) {
    if (member_array(stat, keys(__Stat)) == -1) {
        return;
    }
    __Stat[stat] = n;
    this_object()->update_vitals();
    this_object()->update_limbs();
}
void add_stat_bonus (string stat, int n) {
    initialize_stats();
    if (member_array(stat, keys(__StatBonus)) == -1) {
        return;
    }
    __StatBonus[stat] += n;
}