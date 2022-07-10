nosave private mapping __Class = ([
    "warrior": ([       // str
        "vitals": ([
            "hp": 3,
            "sp": 1,
            "mp": 0,
        ]),
        "stats": ([
            "strength": 3,
            "perception": 1,
            "endurance": 2,
            "charisma": 0,
            "intelligence": 0,
            "agility": 2,
            "luck": 0,
        ]),
        "skills": ([

        ]),
    ]),
    "templar": ([       // str and agi
        "vitals": ([
            "hp": 2,
            "sp": 2,
            "mp": 0,
        ]),
        "stats": ([
            "strength": 2,
            "perception": 1,
            "endurance": 2,
            "charisma": 1,
            "intelligence": 0,
            "agility": 2,
            "luck": 0,
        ]),
        "skills": ([

        ]),
    ]),
    "scoundrel": ([     // agi
        "vitals": ([
            "hp": 1,
            "sp": 3,
            "mp": 0,
        ]),
        "stats": ([
            "strength": 2,
            "perception": 1,
            "endurance": 1,
            "charisma": 1,
            "intelligence": 0,
            "agility": 3,
            "luck": 0,
        ]),
        "skills": ([

        ]),
    ]),
    "ranger": ([        // agi and int
        "vitals": ([
            "hp": 0,
            "sp": 2,
            "mp": 2,
        ]),
        "stats": ([
            "strength": 1,
            "perception": 1,
            "endurance": 1,
            "charisma": 1,
            "intelligence": 2,
            "agility": 2,
            "luck": 0,
        ]),
        "skills": ([

        ]),
    ]),
    "mentalist": ([     // int
        "vitals": ([
            "hp": 0,
            "sp": 1,
            "mp": 3,
        ]),
        "stats": ([
            "strength": 0,
            "perception": 2,
            "endurance": 1,
            "charisma": 1,
            "intelligence": 3,
            "agility": 0,
            "luck": 1,
        ]),
        "skills": ([

        ]),
    ]),
    "paladin": ([       // int and str
        "vitals": ([
            "hp": 2,
            "sp": 0,
            "mp": 2,
        ]),
        "stats": ([
            "strength": 2,
            "perception": 1,
            "endurance": 2,
            "charisma": 1,
            "intelligence": 2,
            "agility": 0,
            "luck": 0,
        ]),
        "skills": ([

        ]),
    ]),
]);

int query_adjust_hp (string c) {
    if (__Class[c] && __Class[c]["vitals"]) {
        return __Class[c]["vitals"]["hp"];
    } else {
        return 0;
    }
}
int query_adjust_sp (string c) {
    if (__Class[c] && __Class[c]["vitals"]) {
        return __Class[c]["vitals"]["sp"];
    } else {
        return 0;
    }
}
int query_adjust_mp (string c) {
    if (__Class[c] && __Class[c]["vitals"]) {
        return __Class[c]["vitals"]["mp"];
    } else {
        return 0;
    }
}

int query_adjust_stat (string c, string stat) {
    if (__Class[c] && __Class[c]["stats"]) {
        return __Class[c]["stats"][stat];
    } else {
        return 0;
    }
}