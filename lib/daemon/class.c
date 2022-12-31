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
            "blade attack": 3,
            "blade defense": 3,
            "blunt attack": 2,
            "blunt defense": 2,
            "brawl attack": 3,
            "brawl defense": 3,
            "psionic attack": 0,
            "psionic defense": 0,
            "ranged attack": 1,
            "ranged defense": 1,
            "medicine": 0,
            "theurgy": 0,
        ]),
    ]),
    "mystic": ([       // str and agi
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
            "blade attack": 1,
            "blade defense": 1,
            "blunt attack": 2,
            "blunt defense": 2,
            "brawl attack": 3,
            "brawl defense": 3,
            "psionic attack": 1,
            "psionic defense": 1,
            "ranged attack": 1,
            "ranged defense": 1,
            "medicine": 3,
            "theurgy": 3,
        ]),
    ]),
    "scoundrel": ([     // agi
        "vitals": ([
            "hp": 1,
            "sp": 3,
            "mp": 0,
        ]),
        "stats": ([
            "strength": 1,
            "perception": 2,
            "endurance": 2,
            "charisma": 0,
            "intelligence": 0,
            "agility": 3,
            "luck": 0,
        ]),
        "skills": ([
            "blade attack": 3,
            "blade defense": 3,
            "blunt attack": 2,
            "blunt defense": 2,
            "brawl attack": 2,
            "brawl defense": 2,
            "psionic attack": 0,
            "psionic defense": 0,
            "ranged attack": 1,
            "ranged defense": 1,
            "medicine": 1,
            "theurgy": 0,
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
            "blade attack": 2,
            "blade defense": 2,
            "blunt attack": 2,
            "blunt defense": 1,
            "brawl attack": 2,
            "brawl defense": 2,
            "psionic attack": 1,
            "psionic defense": 1,
            "ranged attack": 3,
            "ranged defense": 3,
            "medicine": 1,
            "theurgy": 0,
        ]),
    ]),
    "psionist": ([      // int
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
            "blade attack": 0,
            "blade defense": 0,
            "blunt attack": 2,
            "blunt defense": 2,
            "brawl attack": 1,
            "brawl defense": 1,
            "psionic attack": 3,
            "psionic defense": 3,
            "ranged attack": 2,
            "ranged defense": 2,
            "medicine": 1,
            "theurgy": 0,
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
            "blade attack": 1,
            "blade defense": 1,
            "blunt attack": 2,
            "blunt defense": 2,
            "brawl attack": 1,
            "brawl defense": 1,
            "psionic attack": 2,
            "psionic defense": 2,
            "ranged attack": 2,
            "ranged defense": 2,
            "medicine": 2,
            "theurgy": 2,
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

int query_adjust_skill (string c, string skill) {
    if (__Class[c] && __Class[c]["skills"]) {
        return __Class[c]["skills"][skill];
    } else {
        return 0;
    }
}

int query_max_stat (string c, string stat, int level) {
    return level * (2 + query_adjust_stat(c, stat));
}