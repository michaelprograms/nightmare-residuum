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
    float nT, nG, nH, nM;
    int size, size2, size9_10;
    string name, biome;

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
    nT = (noise_simplex_4d(nx, ny, nz, nw, __PCache[name], 5, 1.25) + 1) / 2;
    nT = (nT - 0.25) / (0.75 - 0.25); // normalize 0-1 from 0.25-0.75 (t - min) / (max - min)

    // noise Heat
    nH = (noise_simplex_4d(nx, ny, nz, nw, __PCache[name], 3, 2.0) + 1) / 2;
    nH = (nH - 0.25) / (0.75 - 0.25); // normalize 0-1 from 0.25-0.75 (t - min) / (max - min)

    // noise Gradient
    if (y >= size9_10 && y <= size - size9_10) {  // center 20%
        nG = 1.0;
    } else if (y < size9_10) {    // north 40%
        nG = 1.0 - (gradient_2d(1.0, 1.0, 1.0, 0.0, 0.0, (y * 1.0) / size9_10));
    } else {                        // south 40%
        nG = 1.0 - (gradient_2d(1.0, 1.0, 1.0, 0.0, 0.0, (size - (y + 1.0)) / size9_10));
    }
    nH = (nH * 0.8) * (nG * 1.5);   // TEMPERATURE_MULTIPLIER

    // adjust noise Heat based upon noise Terrain, higher is colder
    if (nT > 0.6 && nT <= 0.7) {        // Grass
        nH -= 0.1 * nT;
    } else if (nT > 0.7 && nT <= 0.8) { // Forest
        nH -= 0.2 * nT;
    } else if (nT > 0.8 && nT <= 0.9) { // Rock
        nH -= 0.3 * nT;
    } else if (nT > 0.9 && nT <= 1.0) { // Snow
        nH -= 0.4 * nT;
    }

    // noise Moisture
    nM = (noise_simplex_4d(nx, ny, nz, nw, __PCache[name], 4, 3.0) + 1) / 2;
    nM = (nM - 0.25) / (0.75 - 0.25); // normalize 0-1 from 0.25-0.75 (t - min) / (max - min)

    // adjust noise Moisture based upon noise Terrain, lower is wetter
    if (nT > 0 && nT <= 0.3) {                    // Deep Water
        nM += 8.0 * nT;
    } else if (nT > 0.3 && nT <= 0.5) { // Shallow Water
        nM += 3.0 * nT;
    } else if (nT > 0.5 && nT <= 0.6) { // Sand
        nM += 0.25 * nT;
    }

    // determine biome
    if (nT <= 0.3) {
        // DEEP WATER
        biome = "deep water";
    } else if (nT <= 0.5) {
        // SHALLOW WATER
        biome = "shallow water";
    } else if (nH <= 0.05) {
        // COLDEST
        // DRYEST-WETTEST: ICE
        biome = "ice";
    } else if (nH <= 0.2) {
        // COLDER
        // DRYEST-WETTEST: TUNDRA
        biome = "tundra";
    } else if (nH <= 0.4) {
        // COLD
        if (nM < 0.5) {
            // DRYEST-DRYER: GRASSLAND
            biome = "grassland";
        } else if (nM < 0.7) {
            // DRY: WOODLAND
            biome = "woodland";
        } else {
            // WET-WETTEST: BOREAL FOREST
            biome = "boreal forest";
        }
    } else if (nH <= 0.5) {
        // HOT
        if (nM < 0.5) {
            // DRYEST-DRYER: DESERT
            biome = "desert";
        } else if (nM < 0.8) {
            // DRY-WET: WOODLAND
            biome = "woodland";
        } else if (nM < 0.9) {
            // WETTER: SEASONAL FOREST
            biome = "seasonal forest";
        } else {
            // WETTEST: TEMPERATE RAINFOREST
            biome = "temperate rainforest";
        }
    } else if (nH <= 0.7) {
        // HOTTER
        if (nM < 0.5) {
            // DRYEST-DRYER: DESERT
            biome = "desert";
        } else if (nM < 0.8) {
            // DRY-WET: SAVANNA
            biome = "savanna";
        } else {
            // WETTER-WETTEST: TROPICAL RAINFOREST
            biome = "tropical rainforest";
        }
    } else {
        // HOTTEST
        if (nM < 0.5) {
            // DRYEST-DRYER: DESERT
            biome = "desert";
        } else if (nM < 0.8) {
            // DRY-WET: SAVANNA
            biome = "savanna";
        } else {
            // WETTER-WETTEST: TROPICAL RAINFOREST
            biome = "tropical rainforest";
        }
    }

    room->set_properties(([
        "nT": nT,
        "nH": nH,
        "nM": nM,
    ]));
    room->set_biome(biome);
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