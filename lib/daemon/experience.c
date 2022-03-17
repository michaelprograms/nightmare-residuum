int query_stat_cost (string stat, int level) {
    float adj = 0 + 0; // @TODO class + species modifiers

    if (!stringp(stat)) error("Bad argument 1 to experience->query_stat_cost");
    if (!intp(level)) error("Bad argument 2 to experience->query_stat_cost");

    if (level < 1) {
        return 0;
    }

    return 1000 + to_int(1.0 * pow(level, 3.0) + 1.25 * pow(level, 3.25));
}

int query_skill_cost (int level) {
    // @TODO skill tiers

    if (!intp(level)) error("Bad argument 1 to experience->query_skill_cost");

    if (level < 1) {
        return 0;
    }

    return 100 + to_int(0.25 * pow(level, 3.0) + 0.25 * pow(level, 3.25));
}

// int query_level_cost (int level) {

//     return 0;
// }

int query_value (mixed m) {
    float lvl, exp;

    if (objectp(m) && m->is_living()) {
        lvl = m->query_level() * 1.0;
    } else if (intp(m)) {
        lvl = m * 1.0;
    } else {
        error("Bad argument 1 to experience->query_value");
    }

    if (lvl < 1.0) {
        return 0;
    }

    // (lvl * 10) + (0.25 * (lvl ^ 2.5)) + (0.5 * (lvl ^ 2.75))
    exp = (lvl * 10) + (0.25 * pow(lvl, 2.5)) + (0.5 * pow(lvl, 2.75));
    return to_int(exp);
}

