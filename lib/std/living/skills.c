#include "living.h"

mapping __Skills = query_default_skills();

private mapping query_default_skills () {
    return ([
        "blade attack": ([ "level": 0, "points": 0, ]),
        "blade defense": ([ "level": 0, "points": 0, ]),
        "blunt attack": ([ "level": 0, "points": 0, ]),
        "blunt defense": ([ "level": 0, "points": 0, ]),
        "brawl attack": ([ "level": 0, "points": 0, ]),
        "brawl defense": ([ "level": 0, "points": 0, ]),
        "psionic attack": ([ "level": 0, "points": 0, ]),
        "psionic defense": ([ "level": 0, "points": 0, ]),
        "ranged attack": ([ "level": 0, "points": 0, ]),
        "ranged defense": ([ "level": 0, "points": 0, ]),
        "medicine": ([ "level": 0, "points": 0, ]),
        "theurgy": ([ "level": 0, "points": 0, ]),
        "anatomy": ([ "level": 0, "points": 0, ]),
    ]);
}

private int query_skill_next_points (string skill) {
    if (!__Skills) __Skills = query_default_skills();
    if (!__Skills[skill]) return 0;
    return D_EXPERIENCE->query_skill_cost(skill, __Skills[skill]["level"], this_object()->query_class(), this_object()->query_species());
}

string *query_all_skills () {
    if (!__Skills) __Skills = query_default_skills();
    return keys(__Skills);
}

string query_skill_progress (string skill) {
    int next;

    if (!__Skills) __Skills = query_default_skills();
    if (!__Skills[skill]) return 0;

    // @TODO if level < max level
    next = query_skill_next_points(skill);
    return to_int(__Skills[skill]["points"] * 100.0 / next) + "%";
}

int query_skill (string skill) {
    if (!__Skills) __Skills = query_default_skills();
    return __Skills[skill]["level"];
}

void set_skill (string skill, int level) {
    if (!__Skills) __Skills = query_default_skills();

    if (!__Skills[skill]) {
        __Skills[skill] = ([ "level": level, "points": 0, ]);
    } else {
        __Skills[skill]["level"] = level;
        __Skills[skill]["points"] = 0;
    }
}

varargs void train_skill (string skill, float multiplier) {
    int next, train;

    if (!__Skills) __Skills = query_default_skills();
    if (!__Skills[skill]) return;

    next = query_skill_next_points(skill);
    if (!multiplier) multiplier = 1.0;
    train = to_int((1 + (this_object()->query_level() / 2) + __Skills[skill]["level"]) * multiplier);
    __Skills[skill]["points"] += max(({train, 1}));

    if (__Skills[skill]["points"] >= next) {
        __Skills[skill]["points"] = __Skills[skill]["points"] - next;
        // @TODO if level < max level
        __Skills[skill]["level"] = __Skills[skill]["level"] + 1;
        message("system", "%^YELLOW%^You feel more experienced in the " + skill + " skill.%^RESET%^\n", this_object());
    }
}