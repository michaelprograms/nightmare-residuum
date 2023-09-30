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

/* ----- biome ----- */

string query_biome (float height, float heat, float humidity) {
    string biome;

    if (height <= 0.3) {
        // DEEPER WATER
        biome = "deeper water";
    } else if (height <= 0.4) {
        // DEEP WATER
        biome = "deep water";
    } else if (height <= 0.5) {
        // SHALLOW WATER
        biome = "shallow water";
    } else if (heat <= 0.05) {
        // COLDEST
        // DRYEST-WETTEST: ICE
        biome = "ice";
    } else if (heat <= 0.2) {
        // COLDER
        // DRYEST-WETTEST: TUNDRA
        biome = "tundra";
    } else if (heat <= 0.4) {
        // COLD
        if (humidity < 0.5) {
            // DRYEST-DRYER: GRASSLAND
            biome = "grassland";
        } else if (humidity < 0.7) {
            // DRY: WOODLAND
            biome = "woodland";
        } else {
            // WET-WETTEST: BOREAL FOREST
            biome = "boreal forest";
        }
    } else if (heat <= 0.5) {
        // HOT
        if (humidity < 0.5) {
            // DRYEST-DRYER: DESERT
            biome = "desert";
        } else if (humidity < 0.8) {
            // DRY-WET: WOODLAND
            biome = "woodland";
        } else if (humidity < 0.9) {
            // WETTER: SEASONAL FOREST
            biome = "seasonal forest";
        } else {
            // WETTEST: TEMPERATE RAINFOREST
            biome = "temperate rainforest";
        }
    } else if (heat <= 0.7) {
        // HOTTER
        if (humidity < 0.5) {
            // DRYEST-DRYER: DESERT
            biome = "desert";
        } else if (humidity < 0.8) {
            // DRY-WET: SAVANNA
            biome = "savanna";
        } else {
            // WETTER-WETTEST: TROPICAL RAINFOREST
            biome = "tropical rainforest";
        }
    } else {
        // HOTTEST
        if (humidity < 0.5) {
            // DRYEST-DRYER: DESERT
            biome = "desert";
        } else if (humidity < 0.8) {
            // DRY-WET: SAVANNA
            biome = "savanna";
        } else {
            // WETTER-WETTEST: TROPICAL RAINFOREST
            biome = "tropical rainforest";
        }
    }

    return biome;
}
string query_biome_color_ansi (string biome) {
    switch (biome) {
        case "deeper water":            return "\e[38;2;0;0;96m";
        case "deep water":              return "\e[38;2;0;0;128m";
        case "shallow water":           return "\e[38;2;25;25;150m";
        case "ice":                     return "\e[38;2;0;0;0m";
        case "tundra":                  return "\e[38;2;96;131;112m";
        case "grassland":               return "\e[38;2;164;255;99m";
        case "woodland":                return "\e[38;2;139;175;90m";
        case "boreal forest":           return "\e[38;2;95;115;62m";
        case "desert":                  return "\e[38;2;238;218;130m";
        case "seasonal forest":         return "\e[38;2;73;100;35m";
        case "temperate rainforest":    return "\e[38;2;29;73;40m";
        case "savanna":                 return "\e[38;2;177;209;110m";
        case "tropical rainforest":     return "\e[38;2;66;123;25m";
        default:                        return "\e[38;2;128;0;0m;";
        }
}
string query_biome_color_hex (string biome) {
    switch (biome) {
        case "deeper water":            return "#000060";
        case "deep water":              return "#000080";
        case "shallow water":           return "#191996";
        case "ice":                     return "#FFFFFF";
        case "tundra":                  return "#608370";
        case "grassland":               return "#A4FF63";
        case "woodland":                return "#8BAF5A";
        case "boreal forest":           return "#5F733E";
        case "desert":                  return "#EEDA82";
        case "seasonal forest":         return "#496423";
        case "temperate rainforest":    return "#1D4928";
        case "savanna":                 return "#B1D16E";
        case "tropical rainforest":     return "#427B19";
        default:                        return "#800000";
    }
}

/* ----- export /tmp/name.json ----- */

#define PI 3.141592653589793
#define PIx2 6.283185307179586

void generate_simplex_json (string name) {
    int x, y, size, size2, size9_10;
    mapping p;
    string line;
    float nx, ny, nz, nw;
    float nHeight, nHeat, nHumidity, nTmp;
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

            // noise Height
            nHeight = (noise_simplex_4d(nx, ny, nz, nw, p, 5, 1.25) + 1) / 2;
            if (nHeight < min) min = nHeight;
            if (nHeight > max) max = nHeight;
            nHeight = (nHeight - 0.25) / (0.75 - 0.25); // normalize 0-1 from 0.25-0.75 (t - min) / (max - min)

            // noise Heat
            nHeat = (noise_simplex_4d(nx, ny, nz, nw, p, 3, 2.0) + 1) / 2;
            nHeat = (nHeat - 0.25) / (0.75 - 0.25); // normalize 0-1 from 0.25-0.75 (t - min) / (max - min)

            // noise Gradient
            if (y >= size9_10 && y <= size - size9_10) {  // center 20%
                nTmp = 1.0;
            } else if (y < size9_10) {    // north 40%
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
            nHumidity = (noise_simplex_4d(nx, ny, nz, nw, p, 4, 3.0) + 1) / 2;
            nHumidity = (nHumidity - 0.25) / (0.75 - 0.25); // normalize 0-1 from 0.25-0.75 (t - min) / (max - min)

            // adjust noise Humidity based upon noise Height, lower is wetter
            if (nHeight > 0 && nHeight <= 0.3) {                    // Deep Water
                nHumidity += 8.0 * nHeight;
            } else if (nHeight > 0.3 && nHeight <= 0.5) { // Shallow Water
                nHumidity += 3.0 * nHeight;
            } else if (nHeight > 0.5 && nHeight <= 0.6) { // Sand
                nHumidity += 0.25 * nHeight;
            }

            // noise River
            if (nHeat > 0.05) {
                nTmp = (((noise_simplex_4d(nx, ny, nz, nw, p, 4, 1.25) + 1) / 2) - 0.25) / (0.75 - 0.25); // normalize 0-1 from 0.25-0.75 (t - min) / (max - min)
                if (nTmp >= 0.72 && nTmp <= 0.78) {
                    nHeight *= 0.5;
                } else {
                    nTmp = (((noise_simplex_4d(nw, nz, ny, nx, p, 5, 1.25) + 1) / 2) - 0.25) / (0.75 - 0.25); // normalize 0-1 from 0.25-0.75 (t - min) / (max - min)
                    if (nTmp >= 0.575 && nTmp <= 0.625) {
                        nHeight *= 0.7;
                    }
                }
            }

            line += "\"" + query_biome_color_hex(query_biome(nHeight, nHeat, nHumidity)) + "\"";

            if (x < size-1) line += ",";
        }
        line += "]" + (y == size - 1 ? "" : ",");
        write_file("/tmp/"+name+".json", line + "\n");
    }
    write_file("/tmp/"+name+".json", "    ],\n    \"min\":\""+min+"\",\n    \"max\":\""+max+"\"\n}");
    write("Seed '"+name+"' size "+size+" done\n");
}