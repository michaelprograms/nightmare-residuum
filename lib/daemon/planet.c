inherit M_CLEAN;

private mapping __Planet = ([
/*
    Data Format:
    "name": string,
    "size": integer,

    "overrides": ([
        "x": integer,
        "y": integer,
        "type": string,
        "dir": string,
        "room": string,

        Examples:
        (["x":x,"y":y,"type":"enter","dir":dir,"room":room,,"desc":"$DIR can be entered.",]),
        (["x":x,"y":y,"type":"blocked","dir":dir,"desc":"$DIR is blocked.",]),
    ]),
*/
]);

/* ----- data ----- */

mapping query_planet (string name) {
    string path = "/save/planet/" + lower_case(name[0..0]) + "/" + name + ".o";
    __Planet = ([ ]);
    restore_object(path);
    return __Planet;
}

int query_planet_size (string name) {
    string path = "/save/planet/" + lower_case(name[0..0]) + "/" + name + ".o";
    __Planet = ([ ]);
    restore_object(path);
    return __Planet["size"] ? __Planet["size"] : 128;
}

int create_planet (string name, mapping config) {
    string path = "/save/planet/" + lower_case(name[0..0]) + "/" + name + ".o";
    if (file_size(path) > 0) {
        return 0;
    }
    config["name"] = name;
    __Planet = config;
    return save_object(path);
}

int adjust_planet (string name, mapping config) {
    string path = "/save/planet/" + lower_case(name[0..0]) + "/" + name + ".o";
    if (file_size(path) == 0) {
        return 0;
    }
    __Planet = ([ ]);
    map_delete(config, "name");
    restore_object(path);
    __Planet += config;
    return save_object(path);
}

/* ----- export /tmp/name.json ----- */

#define PI 3.141592653589793
#define PIx2 6.283185307179586

void generate_simplex_json (string name) {
    int x, y, size, size2, size9_10;
    mapping p;
    string line;
    float nx, ny, nz, nw, nT, nG, nH, nM;
    float min = 1, max = -1;

    size = query_planet_size(name);
    size2 = size/2;
    size9_10 = size2 * 9 / 10;

    p = noise_generate_permutation_simplex(name);

    write_file("/tmp/"+name+".json", "{\n    \"name\":\""+name+"\",\n    \"size\":\""+size+"\",\n    \"data\":[\n", 1);
    for (y = 0; y < size; y ++) {
        line = "        [ ";
        for (x = 0; x < size; x ++) {
            // Calculate our 4D coordinates
            nx = nz = to_float(x);
            ny = nw = to_float(y);

            nx = cos((nx / size) * PIx2) * 2 / PIx2;
            ny = cos((ny / size) * PIx2) * 2 / PIx2;
            nz = sin((nz / size) * PIx2) * 2 / PIx2;
            nw = sin((nw / size) * PIx2) * 2 / PIx2;

            // noise Terrain
            nT = (noise_simplex_4d(nx, ny, nz, nw, p, 5, 1.25) + 1) / 2;
            if (nT < min) min = nT;
            if (nT > max) max = nT;
            nT = (nT - 0.25) / (0.75 - 0.25); // normalize 0-1 from 0.25-0.75 (t - min) / (max - min)

            // noise Heat
            nH = (noise_simplex_4d(nx, ny, nz, nw, p, 3, 2.0) + 1) / 2;
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
            nM = (noise_simplex_4d(nx, ny, nz, nw, p, 4, 3.0) + 1) / 2;
            nM = (nM - 0.25) / (0.75 - 0.25); // normalize 0-1 from 0.25-0.75 (t - min) / (max - min)

            // adjust noise Moisture based upon noise Terrain, lower is wetter
            if (nT > 0 && nT <= 0.3) {                    // Deep Water
                nM += 8.0 * nT;
            } else if (nT > 0.3 && nT <= 0.5) { // Shallow Water
                nM += 3.0 * nT;
            } else if (nT > 0.5 && nT <= 0.6) { // Sand
                nM += 0.25 * nT;
            }

            if (nT <= 0.3) {
                // DEEP WATER
                line += "\"#000080\"";
            } else if (nT <= 0.5) {
                // SHALLOW WATER
                line += "\"#191996\"";
            } else if (nH <= 0.05) {
                // COLDEST
                // DRYEST-WETTEST: ICE
                line += "\"#FFFFFF\"";
            } else if (nH <= 0.2) {
                // COLDER
                // DRYEST-WETTEST: TUNDRA
                line += "\"#608370\"";
            } else if (nH <= 0.4) {
                // COLD
                if (nM < 0.5) {
                    // DRYEST-DRYER: GRASSLAND
                    line += "\"#A4FF63\"";
                } else if (nM < 0.7) {
                    // DRY: WOODLAND
                    line += "\"#8BAF5A\"";
                } else {
                    // WET-WETTEST: BOREAL FOREST
                    line += "\"#5F733E\"";
                }
            } else if (nH <= 0.5) {
                // HOT
                if (nM < 0.5) {
                    // DRYEST-DRYER: DESERT
                    line += "\"#EEDA82\"";
                } else if (nM < 0.8) {
                    // DRY-WET: WOODLAND
                    line += "\"#8BAF5A\"";
                } else if (nM < 0.9) {
                    // WETTER: SEASONAL FOREST
                    line += "\"#496423\"";
                } else {
                    // WETTEST: TEMPERATE RAINFOREST
                    line += "\"#1D4928\"";
                }
            } else if (nH <= 0.7) {
                // HOTTER
                if (nM < 0.5) {
                    // DRYEST-DRYER: DESERT
                    line += "\"#EEDA82\"";
                } else if (nM < 0.8) {
                    // DRY-WET: SAVANNA
                    line += "\"#B1D16E\"";
                } else {
                    // WETTER-WETTEST: TROPICAL RAINFOREST
                    line += "\"#427B19\"";
                }
            } else {
                // HOTTEST
                if (nM < 0.5) {
                    // DRYEST-DRYER: DESERT
                    line += "\"#EEDA82\"";
                } else if (nM < 0.8) {
                    // DRY-WET: SAVANNA
                    line += "\"#B1D16E\"";
                } else {
                    // WETTER-WETTEST: TROPICAL RAINFOREST
                    line += "\"#427B19\"";
                }
            }

            if (x < size-1) line += ",";
        }
        line += "]" + (y == size - 1 ? "" : ",");
        write_file("/tmp/"+name+".json", line + "\n");
    }
    write_file("/tmp/"+name+".json", "    ],\n    \"min\":\""+min+"\",\n    \"max\":\""+max+"\"\n}");
    write("Seed '"+name+"' size "+size+" done\n");
}