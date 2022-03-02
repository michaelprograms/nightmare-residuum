#include "living.h"

mapping __Skills = query_default_skills();

private mapping query_default_skills () {
    return ([
        "attack": ([ "level": 0, "points": 0, ]),
        "defense": ([ "level": 0, "points": 0, ]),
        "unarmed": ([ "level": 0, "points": 0, ]),
        "block": ([ "level": 0, "points": 0, ]),
        "evade": ([ "level": 0, "points": 0, ]),
        "parry": ([ "level": 0, "points": 0, ]),
    ]);
}
private int query_skill_next_points (string skill) {
    if (!__Skills) __Skills = query_default_skills();
    if (!__Skills[skill]) {
        return 0;
    }
    return 100 + to_int(0.25 * pow(__Skills[skill]["level"], 3.0)) + (0.25 * pow(__Skills[skill]["level"], 3.25));
}

string *query_all_skills () {
    if (!__Skills) __Skills = query_default_skills();
    return keys(__Skills);
}

int query_skill_progress (string skill) {
    int next;

    if (!__Skills) __Skills = query_default_skills();
    if (!__Skills[skill]) {
        return 0;
    }

    next = query_skill_next_points(skill);
    return to_int(__Skills[skill]["points"] * 100.0 / next);
}

int query_skill (string skill) {
    if (!__Skills) __Skills = query_default_skills();
    return __Skills[skill]["level"];
}

void set_skill (string skill, int level) {
    // @TODO security

    if (!__Skills) __Skills = query_default_skills();

    if (!__Skills[skill]) {
        __Skills[skill] = ([ "level": level, "points": 0, ]);
    } else {
        __Skills[skill]["level"] = level;
        __Skills[skill]["points"] = 0;
    }
}

varargs void train_skill (string skill, float multiplier) {
    int next;

    if (!__Skills) __Skills = query_default_skills();
    if (!__Skills[skill]) {
        return;
    }

    if (!multiplier) multiplier = 1.0;
    __Skills[skill]["points"] += to_int((1 + (query_level() / 5) + __Skills[skill]["level"]) * multiplier);

    next = query_skill_next_points(skill);
    if (__Skills[skill]["points"] >= next) {
        __Skills[skill]["points"] = __Skills[skill]["points"] - next;
        // @TODO if level < max level
        __Skills[skill]["level"] = __Skills[skill]["level"] + 1;
    }

}