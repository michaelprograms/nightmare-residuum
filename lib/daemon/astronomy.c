inherit M_CLEAN;

mapping __Astronomy = ([
/*
    "astronomy file": object,
*/
]);

mapping query_astronomy () {
    return __Astronomy;
}

void handle_room_create (object room) {
    string roomPath, dirPath, aPath;
    string *dirs;

    if (!objectp(room) || !room->is_room()) {
        return;
    }
    roomPath = base_name(room);
    if (!regexp(roomPath, "^/domain/")) return;
    dirs = explode(roomPath, "/")[1..<3];

    dirPath = "/domain";
    foreach (string dir in dirs) {
        dirPath += "/" + dir;

        // astronomy already loaded
        if (__Astronomy[dirPath]) break;

        // check if astronomy exists
        if (file_size(aPath = dirPath + "/astronomy.c") > 0) {
            __Astronomy[dirPath] = clone_object(aPath);
            break;
        }
    }
}

int handle_remove () {
    foreach (string key,object ob in __Astronomy) {
        ob->handle_remove();
    }
    return ::handle_remove();
}

void create () {
    set_no_clean(clean_never());
    set_heart_beat(20);
}

void process () {
    int t = time();
    foreach (string key,object ob in __Astronomy) {
        ob->handle_time(t);
    }
}

void heart_beat () {
    process();
}