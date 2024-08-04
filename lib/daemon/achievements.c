mapping query_achievements_from_room (mixed dest) {
    string roomPath, achievementPath;

    if (objectp(dest) && dest->is_room()) {
        roomPath = base_name(dest);
    } else if (stringp(dest)) {
        roomPath = dest;
    } else {
        return 0;
    }
    if (!regexp(roomPath, "^/domain/")) {
        return 0;
    }
    if (achievementPath = query_file_recursive(roomPath, "achievements")) {
        return achievementPath->query_achievements();
    }
    return 0;
}