#include <planet.h>

inherit STD_ROOM;

#define CHUNK_SIZE 32

int is_virtual_room () { return 1; }

// -----

void add_potential_exit (string name, object room, string dir, int x, int y) {
    string path = file_name();
    path = replace_string(path, "virtual", "virtual/room");
    path = replace_string(path, "terrain", "terrain/" + name);
    room->set_exit(dir, path + "/" + x + "." + y + ".c");
}

void find_and_set_color (object room, string name, int x, int y) {
    mixed *chunk;
    string terrain;
    int cx, cy;

    chunk = D_PLANET->query_planet_chunk(name, x, y);
    if (sizeof(chunk) < 4) {
        return;
    }

    chunk = explode(chunk[4], ",");
    cx = x % CHUNK_SIZE;
    cy = y % CHUNK_SIZE;
    if (sizeof(chunk) < cy || sizeof(chunk[cy]) < cx) {
        return;
    }

    terrain = chunk[cy][cx..cx];

    terrain = upper_case(terrain);

    switch(terrain[0]) {
    case '0'..'3':
        room->set_room_square_color("%^BLUE%^");
        room->set_short("deep water");
        room->set_long("Surrounded by deep water.");
        break;
    case '4'..'5':
        room->set_room_square_color("%^BLUE%^BOLD%^");
        room->set_short("shallow water");
        room->set_long("Surrounded by shallow water.");
        break;
    case '6':
        room->set_room_square_color("%^YELLOW%^");
        room->set_short("arid sands");
        room->set_long("Surrounded by arid sands.");
        break;
    case '7':
        room->set_room_square_color("%^GREEN%^BOLD%^");
        room->set_short("grassy fields");
        room->set_long("Surrounded by grassy fields.");
        break;
    case '8':
        room->set_room_square_color("%^GREEN%^");
        room->set_short("forests");
        room->set_long("Surrounded by forests.");
        break;
    case '9'..'A':
        room->set_room_square_color("%^ORANGE%^");
        room->set_short("mountainous terrain");
        room->set_long("Surrounded by mountainous terrain.");
        break;
    case 'B'..'F':
        room->set_room_square_color("%^BOLD%^");
        room->set_short("snow covered hills");
        room->set_long("Surrounded by snow covered hills.");
        break;
    default:
        room->set_room_square_color("%^RED%^");
    }
}

int find_planet_size (string name) {
    mixed *result = D_PLANET->query_planet(name);
    if (sizeof(result) > 0) {
        result = result[0];
    }
    return sizeof(result) > 2 ? result[2] : 0;
}

object virtual_create (string arg) {
    string name;
    int x, y, size, xw, xe, yn, ys;
    object room;

    if (sscanf(arg, PLANET_V_ROOM + "terrain/%s/%d.%d", name, x, y) != 3) {
        return 0;
    }

    room = new("/domain/Planet/virtual/room/base/terrain.c");

    find_and_set_color(room, name, x, y);

    size = find_planet_size(name);
    xw = x - 1 > -1     ? x - 1 : size - 1;
    xe = x + 1 < size   ? x + 1 : 0;
    yn = y - 1 >= 0     ? y - 1 : size - 1;
    ys = y + 1 < size   ? y + 1 : 0;

    add_potential_exit(name, room, "northwest", xw, yn);
    add_potential_exit(name, room, "north",     x,  yn);
    add_potential_exit(name, room, "northeast", xe, yn);
    add_potential_exit(name, room, "west",      xw, y);
    add_potential_exit(name, room, "east",      xe, y);
    add_potential_exit(name, room, "southwest", xw, ys);
    add_potential_exit(name, room, "south",     x,  ys);
    add_potential_exit(name, room, "southeast", xe, ys);

    return room;
}