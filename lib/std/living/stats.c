private mapping __Stats = ([
    "strength": 0,
    "perception": 0,
    "endurance": 0,
    "charisma": 0,
    "intelligence": 0,
    "agility": 0,
    "luck": 0,
]);
nosave private mapping __StatsBonus = ([
    "strength": 0,
    "perception": 0,
    "endurance": 0,
    "charisma": 0,
    "intelligence": 0,
    "agility": 0,
    "luck": 0,
]);

int query_stat_base (string stat) {
    return __Stats[stat];
}
int query_stat_bonus (string stat) {
    if (!__StatsBonus) {
        __StatsBonus = ([
            "strength": 0,
            "perception": 0,
            "endurance": 0,
            "charisma": 0,
            "intelligence": 0,
            "agility": 0,
            "luck": 0,
        ]);
    }
    return __StatsBonus[stat];
}
int query_stat (string stat) {
    return __Stats[stat] + query_stat_bonus(stat);
}

void set_stat (string stat, int n) {
    __Stats[stat] = n;
    this_object()->update_vitals();
    this_object()->update_limbs();
}
void add_stat_bonus (string stat, int n) {
    if (!__StatsBonus) {
        __StatsBonus = ([
            "strength": 0,
            "perception": 0,
            "endurance": 0,
            "charisma": 0,
            "intelligence": 0,
            "agility": 0,
            "luck": 0,
        ]);
    }
    if (member_array(stat, keys(__StatsBonus)) == -1) {
        return;
    }
    __StatsBonus[stat] += n;
}