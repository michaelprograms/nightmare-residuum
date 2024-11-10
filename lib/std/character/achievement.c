mapping __Achievements = ([ ]);
/*
    Achievement Data format:
    "name": ([
        "done": 0 | 1,
        "flags": "comma,separated,values",
    ])
*/

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

void set_achievement_flag (string str, string flag) {
    string *flags;

    if (!__Achievements[str]) {
        __Achievements[str] = ([
            "done": 0,
            "flags": "",
        ]);
    }
    if (__Achievements[str]["done"] != 1) {
        flags = explode(__Achievements[str]["flags"], ",");
        if (member_array(flag, flags) == -1) {
            flags += ({ flag });
            flags = sort_array(flags, (: strcmp :));
            __Achievements[str]["flags"] = implode(flags, ",");
            // @TODO: CHECK FOR COMPLETION
        }
    }
}
