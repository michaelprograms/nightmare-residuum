#include <planet.h>

#define PI 3.141592653589793
#define PIx2 6.283185307179586

inherit M_CLEAN;

int is_virtual_room () { return 1; }

/* ----- setup virtual room ----- */

mapping __PCache = ([ ]);   // Permutation Cache

void setup_room (object room) {
    string name;
    int x, y;
    mapping planet, noise;
    string biome;

    name = room->query_property("name");
    x = room->query_property("x");
    y = room->query_property("y");

    planet = D_PLANET->query_planet(name);

    if (!__PCache[planet["name"]]) {
        __PCache[planet["name"]] = noise_generate_permutation_simplex(planet["name"]);
    }

    noise = D_PLANET->query_noise(__PCache[planet["name"]], planet["size"], x, y);
    room->set_property("level", noise["level"]);
    room->set_property("height", to_int(noise["height"] * 100));
    room->set_property("heat", to_int(noise["heat"] * 100));
    room->set_property("humidity", to_int(noise["humidity"] * 100));
    if (noise["resource"]) {
        room->set_property("resource", noise["resource"]);
    }

    biome = D_PLANET->query_biome(noise["height"], noise["heat"], noise["humidity"]);
    room->set_property("biome", biome);
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
                room->set_room_bracket_color("%^I_CYAN%^BOLD%^");
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
    room->set_property("name", name);
    room->set_property("x", x);
    room->set_property("y", y);

    setup_room(room);
    if (arrayp(planet["overrides"])) {
        foreach (mapping override in filter(planet["overrides"], (: $1["x"] == $(x) && $1["y"] == $(y) :))) {
            if (override["type"] == "dome") {
                room->set_property("no resource", 1);
                room->set_property("no receive", 1);
            }
        }
    }
    room->update_descriptions();
    room->update_resource();
    setup_exits(room, planet, x, y);

    return room;
}