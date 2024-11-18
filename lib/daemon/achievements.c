mapping __Achievements = ([ ]);

mapping query_achievements_from_room (mixed dest) {
    string roomPath, aPath;
    if (objectp(dest) && dest->is_room()) {
        roomPath = base_name(dest);
    } else if (stringp(dest)) {
        roomPath = dest;
    }
    if (roomPath && regexp(roomPath, "^/domain/")) {
        if (aPath = query_file_recursive(roomPath, "achievements")) {
            __Achievements[aPath] = aPath->query_achievements();
        }
    }
    return aPath ? __Achievements[aPath] : 0;
}

void flag (object tc, string name, string flag) {
    object env = environment(tc);
    mapping achievements = query_achievements_from_room(env);
    mapping *match = filter(achievements, (: $1["name"] == $(name) :));
    mapping matched = sizeof(match) > 0 ? match[0] : 0;

    tc->set_achievement_flag(name, flag, matched["flags"]);
}