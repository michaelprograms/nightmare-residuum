#include <planet.h>

#define PI 3.141592653589793
#define PIx2 6.283185307179586

#define LEVEL_RANGE 25.0

inherit M_CLEAN;

int is_virtual_room () { return 1; }

/* ----- setup virtual room ----- */

mapping __PCache = ([ ]);   // Permutation Cache

void setup_noise (object room, mapping planet, int x, int y) {
    mapping noise;
    int size2, level;

    if (!__PCache[planet["name"]]) {
        __PCache[planet["name"]] = noise_generate_permutation_simplex(planet["name"]);
    }

    // Level
    size2 = planet["size"] / 2;
    level = to_int(sqrt((size2-x) * (size2-x) + (size2-y) * (size2-y)) / (size2 / LEVEL_RANGE));
    room->set_property("level", level);

    // Biome
    noise = D_PLANET->query_noise(__PCache[planet["name"]], planet["size"], x, y);
    room->set_biome(noise);

    // Resource
    room->set_property("resource", D_PLANET->query_noise_resource(__PCache[planet["name"]], planet["size"], x, y));
}

void setup_exits (object room, mapping planet, int x, int y) {
    string path;
    string name;
    int size, xw, xe, yn, ys;

    name = planet["name"];
    size = planet["size"];

    path = PLANET_V_ROOM + "surface/" + name;
    xw = x     > 0    ? x - 1 : size - 1;
    xe = x + 1 < size ? x + 1 : 0;
    yn = y     > 0    ? y - 1 : size - 1;
    ys = y + 1 < size ? y + 1 : 0;

    room->set_exit("northwest", path + "/" + xw + "." + yn + ".c");
    room->set_exit("north",     path + "/" + x  + "." + yn + ".c");
    room->set_exit("northeast", path + "/" + xe + "." + yn + ".c");
    room->set_exit("west",      path + "/" + xw + "." + y  + ".c");
    room->set_exit("east",      path + "/" + xe + "." + y  + ".c");
    room->set_exit("southwest", path + "/" + xw + "." + ys + ".c");
    room->set_exit("south",     path + "/" + x  + "." + ys + ".c");
    room->set_exit("southeast", path + "/" + xe + "." + ys + ".c");

    if (arrayp(planet["overrides"])) {
        foreach (mapping override in filter(planet["overrides"], (: $1["x"] == $(x) && $1["y"] == $(y) :))) {
            if (override["type"] == "enter") {
                room->remove_exit(override["dir"]);
                room->set_exit("enter " + override["dir"], override["room"]);
                room->add_terrain_override(replace_string(override["desc"], "$DIR", override["dir"]));
            } else if (override["type"] == "blocked") {
                room->remove_exit(override["dir"]);
                room->add_terrain_override(replace_string(override["desc"], "$DIR", override["dir"]));
            } else if (override["type"] == "dome") {
                room->set_room_brackets(({ "(", ")" }));
            }
        }
    }
}

/* ----- called by master::compile_object ----- */

object virtual_create (string path) {
    string name;
    int x, y;
    mapping planet;
    object room;

    if (sscanf(path, PLANET_V_ROOM + "surface/%s/%d.%d", name, x, y) != 3) {
        return 0;
    }

    planet = D_PLANET->query_planet(name);
    room = new(PLANET_V_ROOM + "base/terrain.c");

    setup_noise(room, planet, x, y);
    setup_exits(room, planet, x, y);

    return room;
}

/* -----  ----- */

void update_resource (object room) {
    string name;
    int x, y;
    mapping planet;

    name = file_name(room);

    if (sscanf(name, PLANET_V_ROOM + "surface/%s/%d.%d", name, x, y) != 3) {
        return 0;
    }

    room->set_property("resource", D_PLANET->query_noise_resource(__PCache[planet["name"]], planet["size"], x, y));
}