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
            if (__MaxX < strlen(lines[y])) {
                __MaxX = strlen(lines[y]);
            }
            if (undefinedp(__LayoutMap[x])) {
                __LayoutMap[x] = ([]);
            }
            if (undefinedp(__LayoutMap[x][y])) {
                __LayoutMap[x][y] = 0;
            }
            if (lines[y][x..x] != " ") {
                __LayoutMap[x][y] = 1;
            }
        }
    }
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
        path = replace_string(file_name(this_object()), "virtual/", "virtual/room/");
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