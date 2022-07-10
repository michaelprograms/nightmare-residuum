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
            "knife attack": 1,
            "knife defense": 1,
            "melee attack": 3,
            "melee defense": 3,
            "projectile attack": 1,
            "projectile defense": 1,
            "psionic attack": 0,
            "psionic defense": 0,
            "ranged attack": 1,
            "ranged defense": 1,
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
            "blade attack": 3,
            "blade defense": 3,
            "blunt attack": 2,
            "blunt defense": 2,
            "knife attack": 2,
            "knife defense": 2,
            "melee attack": 2,
            "melee defense": 2,
            "projectile attack": 1,
            "projectile defense": 1,
            "psionic attack": 0,
            "psionic defense": 0,
            "ranged attack": 1,
            "ranged defense": 1,
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
            "blade attack": 2,
            "blade defense": 2,
            "blunt attack": 2,
            "blunt defense": 2,
            "knife attack": 3,
            "knife defense": 3,
            "melee attack": 2,
            "melee defense": 2,
            "projectile attack": 1,
            "projectile defense": 1,
            "psionic attack": 0,
            "psionic defense": 0,
            "ranged attack": 1,
            "ranged defense": 1,
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
            "blade attack": 1,
            "blade defense": 1,
            "blunt attack": 1,
            "blunt defense": 1,
            "knife attack": 2,
            "knife defense": 2,
            "melee attack": 2,
            "melee defense": 2,
            "projectile attack": 3,
            "projectile defense": 3,
            "psionic attack": 1,
            "psionic defense": 1,
            "ranged attack": 3,
            "ranged defense": 3,
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
            "blade attack": 0,
            "blade defense": 0,
            "blunt attack": 2,
            "blunt defense": 2,
            "knife attack": 1,
            "knife defense": 1,
            "melee attack": 1,
            "melee defense": 1,
            "projectile attack": 2,
            "projectile defense": 2,
            "psionic attack": 3,
            "psionic defense": 3,
            "ranged attack": 2,
            "ranged defense": 2,
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
            "knife attack": 1,
            "knife defense": 1,
            "melee attack": 1,
            "melee defense": 1,
            "projectile attack": 2,
            "projectile defense": 2,
            "psionic attack": 2,
            "psionic defense": 2,
            "ranged attack": 2,
            "ranged defense": 2,
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