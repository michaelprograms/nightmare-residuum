inherit STD_ROOM;

nosave private string __Server;

void set_server (string server) {
    __Server = server;
}
string query_server () {
    return __Server;
}

string *query_map () {
    string *lines;
    mapping map;
    string strx, stry;
    int thisx, thisy;

    if (!__Server) {
        return 0;
    }
    if (sscanf(split_path(file_name(this_object()))[1], "%s.%s", strx, stry) != 2) {
        return 0;
    }
    thisx = to_int(strx);
    thisy = to_int(stry);

    lines = ({});
    map = __Server->query_layout_map();

    // for (y = 0; y <= __Server->query_max_y(); y ++) {
    //     string *rooms = ({});
    //     for (x = 0; x <= __Server->query_max_x(); x ++) {
    for (int y = thisy - 2; y <= thisy + 2; y ++) {
        string *rooms = ({});
        for (int x = thisx - 2; x <= thisx + 2; x ++) {
            if (map[x] && map[x][y]) {
                if (thisx == x && thisy == y) {
                    rooms += ({ "[X]" });
                } else {
                    rooms += ({ "[ ]" });
                }
            } else {
                rooms += ({ "   " });
            }
        }
        lines += ({ implode(rooms, "") });
    }

    return lines;
}
