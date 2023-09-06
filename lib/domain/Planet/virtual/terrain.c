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

    chunk = D_PLANET->query_planet_chunk(name, x, y);
    if (sizeof(chunk) < 4) {
        return;
    }

    chunk = explode(chunk[4], ",");
    x = x % CHUNK_SIZE;
    y = y % CHUNK_SIZE;
    if (sizeof(chunk) < y-1 || sizeof(chunk[y-1]) < x-1) {
        return;
    }

    terrain = chunk[y-1][x-1..x-1];
    // write(x+", "+y+" results: "+sprintf("%O", chunk[y-1])+"\n");
    // write(x+", "+y+" results: "+sprintf("%O", chunk[y-1][x-1..x-1])+"\n");

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

object virtual_create (string arg) {
    string name;
    int x, y;
    object room;

    if (sscanf(arg, PLANET_V_ROOM + "terrain/%s/%d.%d", name, x, y) != 3) {
        return 0;
    }

    room = new("/domain/Planet/virtual/room/base/terrain.c");

    find_and_set_color(room, name, x, y);

    // @TODO find out size of world
    // @TODO wrap around world axises

    add_potential_exit(name, room, "northwest", x - 1, y - 1);
    add_potential_exit(name, room, "north",     x,     y - 1);
    add_potential_exit(name, room, "northeast", x + 1, y - 1);
    add_potential_exit(name, room, "west",      x - 1, y    );
    add_potential_exit(name, room, "east",      x + 1, y    );
    add_potential_exit(name, room, "southwest", x - 1, y + 1);
    add_potential_exit(name, room, "south",     x,     y + 1);
    add_potential_exit(name, room, "southeast", x + 1, y + 1);

    return room;
}