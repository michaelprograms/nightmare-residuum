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
            __Achievements[aPath] = aPath->query_astronomy();
        }
    }
    return aPath ? __Achievements[aPath] : 0;
}