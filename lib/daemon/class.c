nosave private mapping __Class = ([
    "warrior": ([       // str
        "hp": 3,
        "sp": 1,
        "mp": 0,
        "strength": 3,
        "perception": 1,
        "endurance": 2,
        "charisma": 0,
        "intelligence": 0,
        "agility": 2,
        "luck": 0,
    ]),
    "templar": ([       // str and agi
        "hp": 2,
        "sp": 2,
        "mp": 0,
        "strength": 2,
        "perception": 1,
        "endurance": 2,
        "charisma": 1,
        "intelligence": 0,
        "agility": 2,
        "luck": 0,
    ]),
    "scoundrel": ([     // agi
        "hp": 1,
        "sp": 3,
        "mp": 0,
        "strength": 2,
        "perception": 1,
        "endurance": 1,
        "charisma": 1,
        "intelligence": 0,
        "agility": 3,
        "luck": 0,
    ]),
    "ranger": ([        // agi and int
        "hp": 0,
        "sp": 2,
        "mp": 2,
        "strength": 1,
        "perception": 1,
        "endurance": 1,
        "charisma": 1,
        "intelligence": 2,
        "agility": 2,
        "luck": 0,
    ]),
    "mentalist": ([     // int
        "hp": 0,
        "sp": 1,
        "mp": 3,
        "strength": 0,
        "perception": 2,
        "endurance": 1,
        "charisma": 1,
        "intelligence": 3,
        "agility": 0,
        "luck": 1,
    ]),
    "paladin": ([       // int and str
        "hp": 2,
        "sp": 0,
        "mp": 2,
        "strength": 2,
        "perception": 1,
        "endurance": 2,
        "charisma": 1,
        "intelligence": 2,
        "agility": 0,
        "luck": 0,
    ]),
]);

int query_adjust_hp (string c) {
    return __Class[c] ? __Class[c]["hp"] : 0;
}
int query_adjust_sp (string c) {
    return __Class[c] ? __Class[c]["sp"] : 0;
}
int query_adjust_mp (string c) {
    return __Class[c] ? __Class[c]["mp"] : 0;
}

int query_adjust_stat (string c, string stat) {
    return __Class[c] ? __Class[c][stat] : 0;
}