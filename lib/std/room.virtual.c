inherit STD_ROOM;


nosave private string __Layout;
nosave private mapping __LayoutMap = ([]);
nosave private int __MaxX = -1, __MaxY = -1;
nosave private string __Inheritable;

int is_virtual_room () { return 1; }

string query_layout () {
    return __Layout;
}
void set_layout (string layout) {
    string *lines;

    __Layout = layout;
    lines = explode(__Layout, "\n");
    __MaxY = sizeof(lines) - 1;

    // parse the layout map
    for (int y = 0; y < sizeof(lines); y ++) {
        for (int x = 0; x < sizeof(lines[y]); x ++) {
            if (__MaxX < sizeof(lines[y]) - 1) {
                __MaxX = sizeof(lines[y]) - 1;
            }
            if (!__LayoutMap[x]) {
                __LayoutMap[x] = ([]);
            }
            if (!__LayoutMap[x][y]) {
                __LayoutMap[x][y] = 0;
            }
            if (lines[y][x..x] != " ") {
                __LayoutMap[x][y] = lines[y][x..x];
            }
        }
    }
}
void set_layout_file (string filename) {
    string *lines;
    string *layout = ({});

    if (file_length(filename) < 1) {
        raise_error("Bad argument 1 to room.virtual->set_layout_file");
    }
    lines = explode(read_file(filename), "\n");
    foreach (string line in lines) {
        string l = "";
        foreach (string t in explode(line, ",")) {
            if (t == "0") l += " ";
            else {
                if (t == "10") t = "A";
                else if (t == "11") t = "B";
                else if (t == "12") t = "C";
                else if (t == "13") t = "D";
                else if (t == "14") t = "E";
                else if (t == "15") t = "F";
                l += t;
            }
        }
        layout += ({ l });
    }
    set_layout(implode(layout, "\n"));
}
mapping query_layout_map () {
    return __LayoutMap;
}
int query_within_layout (int x, int y) {
    if (__LayoutMap[x]) {
        return __LayoutMap[x][y];
    } else {
        return 0;
    }
}
int query_max_x () {
    return __MaxX;
}
int query_max_y () {
    return __MaxY;
}

string query_inheritable () {
    return __Inheritable;
}
void set_inheritable (string path) {
    __Inheritable = path;
}

void add_potential_exit (object room, string dir, int x, int y) {
    string path;
    if (query_within_layout(x, y)) {
        path = replace_string(program_name(this_object()), "virtual/", "virtual/room/");
        room->set_exit(dir, path + "/" + x + "." + y);
    }
}

object virtual_create (string arg) {
    int i = strsrch(arg, ".", -1), j = strsrch(arg, "/", -1);
    int x, y;
    object room;

    if (i == -1) {
        return 0;
    }

    x = to_int(arg[j+1..i-1]);
    y = to_int(arg[i+1..]);

    if (!query_within_layout(x, y)) {
        return 0;
    }

    room = new(__Inheritable);
    room->set_server(program_name(this_object()));

    // Setup exits
    add_potential_exit(room, "northwest", x - 1, y - 1);
    add_potential_exit(room, "north",     x,     y - 1);
    add_potential_exit(room, "northeast", x + 1, y - 1);
    add_potential_exit(room, "west",      x - 1, y    );
    add_potential_exit(room, "east",      x + 1, y    );
    add_potential_exit(room, "southwest", x - 1, y + 1);
    add_potential_exit(room, "south",     x,     y + 1);
    add_potential_exit(room, "southeast", x + 1, y + 1);

    return room;
}

varargs string *query_map (string mode) {
    string *lines;
    string strx, stry;
    int thisx, thisy;
    int ax, ay, bx, by;

    if (sscanf(split_path(object_name(previous_object()))[1], "%s.%s", strx, stry) != 2) {
        return 0;
    }
    thisx = to_int(strx);
    thisy = to_int(stry);

    lines = ({});

    if (!mode || mode == "room") {
        ax = thisx - 2;
        bx = thisx + 2;
        ay = thisy - 2;
        by = thisy + 2;
    } else if (mode == "all") {
        ax = max(({ thisx - 15, 0 }));
        bx = min(({ thisx + 15, __MaxX }));
        ay = max(({ thisy - 15, 0 }));
        by = min(({ thisy + 15, __MaxY }));
    }

    for (int y = ay; y <= by; y ++) {
        string *rooms = ({});
        for (int x = ax; x <= bx; x ++) {
            if (__LayoutMap[x] && __LayoutMap[x][y]) {
                string center = (thisx == x && thisy == y ? "X" : " ");
                string color = "", wrapper;
                switch (__LayoutMap[x][y][0]) {
                    case '0'..'3':
                        color = "%^BLUE%^";
                        break;
                    case '4'..'5':
                        color = "%^BLUE%^BOLD%^";
                        break;
                    case '6':
                        color = "%^YELLOW%^";
                        break;
                    case '7':
                        color = "%^GREEN%^BOLD%^";
                        break;
                    case '8':
                        color = "%^GREEN%^";
                        break;
                    case '9'..'A':
                        color = "%^ORANGE%^";
                        break;
                    case 'B'..'F':
                        color = "%^BOLD%^";
                        break;
                    default:
                        color = "%^RED%^";
                }
                rooms += ({ color + (wrapper ? wrapper : "[") + "%^RESET%^" + center + color + (wrapper ? wrapper : "]") + "%^RESET%^" });
            } else {
                rooms += ({ "   " });
            }
        }
        lines += ({ implode(rooms, "") });
    }

    return lines;
}