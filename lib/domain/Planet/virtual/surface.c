#include <planet.h>

#define PI 3.141592653589793
#define PIx2 6.283185307179586

inherit M_CLEAN;

int is_virtual_room () { return 1; }

/* ----- setup virtual room ----- */

// Permutation Cache
mapping __PCache = ([ ]);

void setup_noise (object room, mapping planet, int x, int y) {
    float nx, ny, nz, nw;
    float nHeight, nHeat, nHumidity, nTmp;
    int size, size2, size9_10;
    string name;

    name = planet["name"];
    if (!__PCache[name]) {
        __PCache[name] = noise_generate_permutation_simplex(name);
    }

    size = planet["size"];
    size2 = size/2;
    size9_10 = size2 * 9 / 10;

    nx = nz = to_float(x);
    ny = nw = to_float(y);

    nx = cos((nx / size) * PIx2) * 2 / PIx2;
    ny = cos((ny / size) * PIx2) * 2 / PIx2;
    nz = sin((nz / size) * PIx2) * 2 / PIx2;
    nw = sin((nw / size) * PIx2) * 2 / PIx2;

    // noise Terrain
    nHeight = (noise_simplex_4d(nx, ny, nz, nw, __PCache[name], 5, 1.25) + 1) / 2;
    nHeight = (nHeight - 0.25) / (0.75 - 0.25); // normalize 0-1 from 0.25-0.75 (t - min) / (max - min)

    // noise Heat
    nHeat = (noise_simplex_4d(nx, ny, nz, nw, __PCache[name], 3, 2.0) + 1) / 2;
    nHeat = (nHeat - 0.25) / (0.75 - 0.25); // normalize 0-1 from 0.25-0.75 (t - min) / (max - min)

    // noise Gradient
    if (y >= size9_10 && y <= size - size9_10) {  // center 20%
        nTmp = 1.0;
    } else if (y < size9_10) {      // north 40%
        nTmp = 1.0 - (gradient_2d(1.0, 1.0, 1.0, 0.0, 0.0, (y * 1.0) / size9_10));
    } else {                        // south 40%
        nTmp = 1.0 - (gradient_2d(1.0, 1.0, 1.0, 0.0, 0.0, (size - (y + 1.0)) / size9_10));
    }
    nHeat = (nHeat * 0.8) * (nTmp * 1.5);   // TEMPERATURE_MULTIPLIER

    // adjust noise Heat based upon noise Height, higher is colder
    if (nHeight > 0.6 && nHeight <= 0.7) {        // Grass
        nHeat -= 0.1 * nHeight;
    } else if (nHeight > 0.7 && nHeight <= 0.8) { // Forest
        nHeat -= 0.2 * nHeight;
    } else if (nHeight > 0.8 && nHeight <= 0.9) { // Rock
        nHeat -= 0.3 * nHeight;
    } else if (nHeight > 0.9 && nHeight <= 1.0) { // Snow
        nHeat -= 0.4 * nHeight;
    }

    // noise Humidity
    nHumidity = (noise_simplex_4d(nx, ny, nz, nw, __PCache[name], 4, 3.0) + 1) / 2;
    nHumidity = (nHumidity - 0.25) / (0.75 - 0.25); // normalize 0-1 from 0.25-0.75 (t - min) / (max - min)

    // adjust noise Humidity based upon noise Height, lower is wetter
    if (nHeight > 0 && nHeight <= 0.3) {              // Deep Water
        nHumidity += 8.0 * nHeight;
    } else if (nHeight > 0.3 && nHeight <= 0.5) {     // Shallow Water
        nHumidity += 3.0 * nHeight;
    } else if (nHeight > 0.5 && nHeight <= 0.6) {     // Sand
        nHumidity += 0.25 * nHeight;
    }

    // noise River
    if (nHeat > 0.05) {
        nTmp = (((noise_simplex_4d(nx, ny, nz, nw, __PCache[name], 4, 1.25) + 1) / 2) - 0.25) / (0.75 - 0.25); // normalize 0-1 from 0.25-0.75 (t - min) / (max - min)
        if (nTmp >= 0.72 && nTmp <= 0.78) {
            nHeight *= 0.5;
        } else {
            nTmp = (((noise_simplex_4d(nw, nz, ny, nx, __PCache[name], 5, 1.25) + 1) / 2) - 0.25) / (0.75 - 0.25); // normalize 0-1 from 0.25-0.75 (t - min) / (max - min)
            if (nTmp >= 0.575 && nTmp <= 0.625) {
                nHeight *= 0.7;
            }
        }
    }

    room->set_biome(nHeight, nHeat, nHumidity);
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
            }
        }
    }
}

/* ----- called by master::compile_object ----- */

object virtual_create (string arg) {
    string name;
    int x, y;
    mapping planet;
    object room;

    if (sscanf(arg, PLANET_V_ROOM + "surface/%s/%d.%d", name, x, y) != 3) {
        return 0;
    }

    planet = D_PLANET->query_planet(name);
    room = new(PLANET_V_ROOM + "base/terrain.c");

    setup_noise(room, planet, x, y);
    setup_exits(room, planet, x, y);

    return room;
}