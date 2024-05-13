mapping __Achievements = ([ ]);

private void initialize_achievements () {
    if (!mapp(__Achievements)) {
        __Achievements = ([ ]);
    }
}

int set_achievement (string str) {
    initialize_achievements();
    if (__Achievements[str]) {
        return 0;
    }
    __Achievements[str] = 1;
    return 1;
}
int query_achievement (string str) {
    initialize_achievements();
    return !!__Achievements[str];
}
string *query_achievements () {
    initialize_achievements();
    return keys(__Achievements);
}

void remove_achievement (string str) {
    initialize_achievements();
    map_delete(__Achievements, str);
}