mapping __Achievements = ([ ]);

/**
 * Recursively find an achievements.c from current location
 *
 * @param {"std/room"} dest
 */
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

/**
 * Flags the character's achievement progress
 *
 * @param {STD_CHARACTER} tc - this character
 * @param name - the name of the achievement to flag
 * @param flag - the flag to add to achievement
 */
void flag (object tc, string name, string flag) {
    object env;
    mapping achievements, *match;

    env = environment(tc);
    achievements = this_object()->query_achievements_from_room(env);
    match = filter(achievements, (: $1["name"] == $(name) :));
    if (sizeof(match)) {
        tc->set_achievement_flag(name, flag, match[0]["flags"]);
    }
}