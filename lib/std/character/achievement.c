private mapping __Achievements = ([ ]);

int set_achievement (string str) {
    if (!mapp(__Achievements)) __Achievements = ([ ]);

    if (__Achievements[str]) {
        return 0;
    }
    __Achievements[str] = 1;
    return 1;
}
int query_achievement (string str) {
    if (!mapp(__Achievements)) __Achievements = ([ ]);

    if (__Achievements[str]) {
        return 1;
    } else {
        return 0;
    }
}
string *query_achievements () {
    return keys(__Achievements);
}

void remove_achievement (string str) {
    if (!mapp(__Achievements)) __Achievements = ([ ]);

    map_delete(__Achievements, str);
}