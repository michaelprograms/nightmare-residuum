#define STAT_X1     1.0
#define STAT_Y1     3.0
#define STAT_X2     1.25
#define STAT_Y2     3.25
#define STAT_ADJ(n) (n * 0.01)
#define STAT_BASE   1000

#define LEVEL_X1     2.0
#define LEVEL_Y1     3.0
#define LEVEL_X2     2.0
#define LEVEL_Y2     3.5
#define LEVEL_BASE   100


varargs int query_stat_cost (string stat, int level, string c, string s) {
    int bonus = 0;

    if (!stringp(stat)) error("Bad argument 1 to experience->query_stat_cost");
    if (!intp(level)) error("Bad argument 2 to experience->query_stat_cost");

    if (level < 1) return 1;

    if (stringp(c)) bonus += D_CLASS->query_adjust_stat(c, stat);
    if (stringp(s)) bonus += D_SPECIES->query_adjust_stat(s, stat);

    // (Level * Base) + X1 * (Level ^ (Y1-(Bonus*Adj)) + X2 * (Level ^ (Y2-(Bonus*Adj)))
    return to_int(
        level * STAT_BASE +
        STAT_X1 * pow(1.0 * level, STAT_Y1 - STAT_ADJ(bonus)) +
        STAT_X2 * pow(1.0 * level, STAT_Y2 - STAT_ADJ(bonus))
    );
}

int query_level_cost (int level) {
    if (!intp(level)) error("Bad argument 1 to experience->query_level_cost");

    if (level < 1) return 1;

    // (Level * Base) + X1 * (Level ^ Y1 + X2 * (Level ^ Y2)
    return to_int(
        level * LEVEL_BASE +
        LEVEL_X1 * pow(1.0 * level, LEVEL_Y1) +
        LEVEL_X2 * pow(1.0 * level, LEVEL_Y2)
    );
}

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
        return 1;
    }

    // (lvl * 10) + (0.25 * (lvl ^ 2.5)) + (0.5 * (lvl ^ 2.75))
    exp = (lvl * 10) + (0.25 * pow(lvl, 2.5)) + (0.5 * pow(lvl, 2.75));
    return to_int(exp);
}

