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

int query_achievement_done (string str) {
    initialize_achievements();
    return __Achievements[str] && __Achievements[str]["done"] == 1;
}
string query_achievement (string str) {
    initialize_achievements();
    return __Achievements[str];
}
string *query_achievements_done () {
    initialize_achievements();
    return keys(filter(__Achievements, (: $2["done"] :)));
}
string *query_achievements_incomplete () {
    initialize_achievements();
    return keys(filter(__Achievements, (: !$2["done"] :)));
}
void remove_achievement (string str) {
    initialize_achievements();
    map_delete(__Achievements, str);
}

mapping set_achievement_flag (string str, string flag, string allflags) {
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
            if (__Achievements[str]["flags"] == allflags) {
                map_delete(__Achievements[str], "flags");
                __Achievements[str]["done"] = 1;
                message("action", "\n%^BOLD%^You have completed "+str+"!%^RESET%^\n", this_object());

            }
        }
    }
    return __Achievements[str];
}
