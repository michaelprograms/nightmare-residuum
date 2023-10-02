inherit M_CLEAN;

#define PI                  3.141592653589793
#define PIx2                6.283185307179586

#define HEIGHT_DEEPER       0.280
#define HEIGHT_DEEP         0.380
#define HEIGHT_SHALLOW      0.480
#define HEIGHT_SHORE        0.500

#define HEAT_COLDEST        0.05
#define HEAT_COLDER         0.20
#define HEAT_COLD           0.40
#define HEAT_HOT            0.60
#define HEAT_HOTTER         0.80
#define HEAT_HOTTEST        1.00

#define HUMIDITY_DRYEST     0.20
#define HUMIDITY_DRYER      0.45
#define HUMIDITY_DRY        0.60
#define HUMIDITY_WET        0.80
#define HUMIDITY_WETTER     0.95
// HUMIDITY_WETTEST > 0.95

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

/* ----- noise ----- */

mapping query_noise (mapping p, int size, int x, int y) {
    int size2;
    float nx, ny, nz, nw;
    float nHeight, nHumidity, nHeat, nTmp;

    size2 = size * 45 / 100;

    // Calculate our 4D coordinates
    nx = nz = to_float(x);
    ny = nw = to_float(y);
    nx = cos((nx / size) * PIx2) * 2 / PIx2;
    ny = cos((ny / size) * PIx2) * 2 / PIx2;
    nz = sin((nz / size) * PIx2) * 2 / PIx2;
    nw = sin((nw / size) * PIx2) * 2 / PIx2;

    // noise Height
    nHeight = (((noise_simplex_4d(nx, ny, nz, nw, p, 5, 1.25) + 1) / 2) - 0.25) / 0.5; // normalize 0.25-0.75 to 0-1
    // noise River
    nTmp = (((noise_simplex_4d(nx, ny, nz, nw, p, 4, 1.25) + 1) / 2) - 0.25) / 0.5; // normalize 0.25-0.75 to 0-1
    if (nTmp >= 0.72 && nTmp <= 0.78) {
        if (nHeight < 0.5) {
            nHeight *= 0.75;
        } else {
            nHeight *= 0.65;
        }
    } else {
        nTmp = (((noise_simplex_4d(nw, nz, ny, nx, p, 5, 1.25) + 1) / 2) - 0.25) / 0.5; // normalize 0.25-0.75 to 0-1
        if (nTmp >= 0.575 && nTmp <= 0.625 || nTmp >= 0.275 && nTmp <= 0.325) {
            if (nHeight < 0.5) {
                nHeight *= 0.85;
            } else {
                nHeight *= 0.7;
            }
        }
    }

    // noise Humidity
    nHumidity = max(({
        (((noise_simplex_4d(nx, ny, nz, nw, p, 4, 3.0) + 1) / 2) - 0.25) / 0.5,
        0.0
    })); // normalize 0.25-0.75 to 0-1
    // adjust noise Humidity based upon noise Height, lower is wetter
    if (nHeight <= HEIGHT_DEEPER) {         // Deeper water
        nHumidity += 5.0 * nHeight;
    } else if (nHeight <= HEIGHT_DEEP) {    // Deep water
        nHumidity += 4.0 * nHeight;
    } else if (nHeight <= HEIGHT_SHALLOW) { // Shallow Water
        nHumidity += 3.0 * nHeight;
    } else if (nHeight <= HEIGHT_SHORE) {   // Shore
        nHumidity += 0.25 * nHeight;
    }

    // noise Heat
    nHeat = abs((noise_simplex_4d(nx, ny, nz, nw, p, 4, 2.0) + 1) / 2);
    nHeat = abs((nHeat - 0.05) / (0.95 - 0.05));
    // noise Gradient
    if (y >= size2 && y <= size - size2) {  // center 20%
        nTmp = 1.0;
    } else if (y < size2) {      // north 40%
        nTmp = 1.0 - (gradient_2d(1.0, 1.0, 1.0, 0.0, 0.0, (y * 1.0) / size2));
    } else {                        // south 40%
        nTmp = 1.0 - (gradient_2d(1.0, 1.0, 1.0, 0.0, 0.0, (size - (y + 1.0)) / size2));
    }
    nHeat = nHeat * nTmp * 1.5; // @TODO * HEAT_FACTOR; // HEAT_FACTOR = 1.0
    // adjust noise Heat based upon noise Height, higher is colder
    if (nHeight > HEIGHT_SHALLOW) {
        nHeat -= (nHeight - HEIGHT_SHALLOW) * nHeight;
    }
    nHeat = max(({ nHeat, 0.0 }));

    return ([
        "height": nHeight,
        "humidity": nHumidity,
        "heat": nHeat,
    ]);
}

/* ----- biome ----- */

/*
    HEIGHT <= 0.5:
            COLDEST COLDER  COLD        HOT         HOTTER      HOTTEST
    DEEPER  ICY     DEEPER ->
    DEEP    ICY     DEEP ->
    SHALLOW ICY     SHALLOW ->

    HEIGHT > 0.5:
            COLDEST COLDER  COLD        HOT         HOTTER      HOTTEST
    DRYEST  ICE     TUNDRA  GRASSLAND   DESERT      DESERT      DESERT

    DRYER   ICE     TUNDRA  GRASSLAND   DESERT      DESERT      DESERT

    DRY     ICE     TUNDRA  WOODLAND    WOODLAND    SAVANNA     SAVANNA

    WET     ICE     TUNDRA  BOREAL      TEMPERATE   SAVANNA     SAVANNA
                            FOREST      FOREST
    WETTER  ICE     TUNDRA  BOREAL      TEMPERATE   TROPICAL    TROPICAL
                            FOREST      FOREST      RAINFOREST  RAINFOREST
    WETTEST ICE     TUNDRA  BOREAL      TEMPERATE   TROPICAL    TROPICAL
                            FOREST      RAINFOREST  RAINFOREST  RAINFOREST
*/
string query_biome (float height, float heat, float humidity) {
    string biome;

    if (height <= HEIGHT_DEEPER) {
        if (heat <= HEAT_COLDEST) {
            // frozen WATER
            biome = "frozen water";
        } else if (heat <= HEAT_COLDER) {
            // ICY WATER
            biome = "icy water";
        } else {
            // DEEPER WATER
            biome = "deeper water";
        }
    } else if (height <= HEIGHT_DEEP) {
        if (heat <= HEAT_COLDEST) {
            // frozen WATER
            biome = "frozen water";
        } else if (heat <= HEAT_COLDER) {
            // ICY WATER
            biome = "icy water";
        } else {
            // DEEP WATER
            biome = "deep water";
        }
    } else if (height <= HEIGHT_SHALLOW) {
        if (heat <= HEAT_COLDEST) {
            // frozen WATER
            biome = "frozen water";
        } else if (heat <= HEAT_COLDER) {
            // ICY WATER
            biome = "icy water";
        } else {
            // SHALLOW WATER
            biome = "shallow water";
        }
    } else if (heat <= HEAT_COLDEST) {
        // DRYEST-WETTEST: ICE
        biome = "ice";
    } else if (heat <= HEAT_COLDER) {
        // DRYEST-WETTEST: TUNDRA
        biome = "tundra";
    } else if (heat <= HEAT_COLD) {
        if (humidity <= HUMIDITY_DRYER) {
            // DRYEST-DRYER: GRASSLAND
            biome = "grassland";
        } else if (humidity <= HUMIDITY_DRY) {
            // DRY: WOODLAND
            biome = "woodland";
        } else {
            // WET-WETTEST: BOREAL FOREST
            biome = "boreal forest";
        }
    } else if (heat <= HEAT_HOT) {
        if (humidity <= HUMIDITY_DRYER) {
            // DRYEST-DRYER: DESERT
            biome = "desert";
        } else if (humidity <= HUMIDITY_DRY) {
            // DRY: WOODLAND
            biome = "woodland";
        } else {
            // WET-WETTEST: TEMPERATE RAINFOREST
            biome = "temperate rainforest";
        }
    } else if (heat <= HEAT_HOTTER) {
        if (humidity <= HUMIDITY_DRYER) {
            // DRYEST-DRYER: DESERT
            biome = "desert";
        } else if (humidity <= HUMIDITY_WET) {
            // DRY-WET: SAVANNA
            biome = "savanna";
        } else {
            // WETTER-WETTEST: TROPICAL RAINFOREST
            biome = "tropical rainforest";
        }
    } else { // HEAT_HOTTEST
        if (humidity <= HUMIDITY_DRYER) {
            // DRYEST-DRYER: DESERT
            biome = "desert";
        } else if (humidity <= HUMIDITY_WET) {
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
        case "icy water":               return "\e[38;2;105;189;230m";
        case "frozen water":            return "\e[38;2;189;219;246m";
        case "ice":                     return "\e[38;2;255;255;255m";
        case "tundra":                  return "\e[38;2;96;131;112m";
        case "grassland":               return "\e[38;2;164;255;99m";
        case "woodland":                return "\e[38;2;139;175;90m";
        case "boreal forest":           return "\e[38;2;95;115;62m";
        case "desert":                  return "\e[38;2;238;218;130m";
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
        case "icy water":               return "#69BDE6";
        case "frozen water":            return "#BDDBF6";
        case "ice":                     return "#FFFFFF";
        case "tundra":                  return "#608370";
        case "grassland":               return "#A4FF63";
        case "woodland":                return "#8BAF5A";
        case "boreal forest":           return "#5F733E";
        case "desert":                  return "#EEDA82";
        case "temperate rainforest":    return "#1D4928";
        case "savanna":                 return "#B1D16E";
        case "tropical rainforest":     return "#427B19";
        default:                        return "#800000";
    }
}
string query_humidity_color_hex (float humidity) {
    if (humidity <= HUMIDITY_DRYEST)        return "#FF8B11"; // 255;139;17
    else if (humidity <= HUMIDITY_DRYER)    return "#F5F511"; // 245;245;17
    else if (humidity <= HUMIDITY_DRY)      return "#50FF00"; // 80;255;0
    else if (humidity <= HUMIDITY_WET)      return "#55FFFF"; // 85;255;255
    else if (humidity <= HUMIDITY_WETTER)   return "#1446FF"; // 20;70;255
    else                                    return "#000064"; // 0;0;100
}
string query_heat_color_hex (float heat) {
    if (heat <= HEAT_COLDEST)       return "#00FFFF"; // 0;255;255
    else if (heat <= HEAT_COLDER)   return "#AAFFFF"; // 170;255;255
    else if (heat <= HEAT_COLD)     return "#00E585"; // 0;229;133
    else if (heat <= HEAT_HOT)      return "#FFFF64"; // 255;255;100
    else if (heat <= HEAT_HOTTER)   return "#FF6400"; // 255;100;0
    else                            return "#F10C00"; // 241;12;0
}

/* ----- export /tmp/name.json ----- */

void generate_simplex_json (string name) {
    int x, y, size;
    mapping p;
    string line, biome;
    float height_min = 1, height_max = -1, heat_min = 1, heat_max = -1, humidity_min = 1, humidity_max = -1;
    mapping n;
    mapping biomes = ([
        "frozen water": 0,
        "icy water": 0,
        "deeper water": 0,
        "deep water": 0,
        "shallow water": 0,
        "ice": 0,
        "tundra": 0,
        "grassland": 0,
        "woodland": 0,
        "boreal forest": 0,
        "desert": 0,
        "temperate rainforest": 0,
        "savanna": 0,
        "tropical rainforest": 0,
    ]);

    size = query_planet_size(name);
    p = noise_generate_permutation_simplex(name);

    write_file("/tmp/"+name+".json", "{\n    \"name\":\""+name+"\",\n    \"size\":\""+size+"\",\n    \"data\":[\n", 1);
    for (y = 0; y < size; y ++) {
        line = "        [ ";
        for (x = 0; x < size; x ++) {
            n = query_noise(p, size, x, y);

            if (n["height"] < height_min) height_min = n["height"];
            if (n["height"] > height_max) height_max = n["height"];
            if (n["humidity"] < humidity_min) humidity_min = n["humidity"];
            if (n["humidity"] > humidity_max) humidity_max = n["humidity"];
            if (n["heat"] < heat_min) heat_min = n["heat"];
            if (n["heat"] > heat_max) heat_max = n["heat"];

            biome = query_biome(n["height"], n["heat"], n["humidity"]);
            biomes[biome] ++;
            line += "[ " +
                "\"" + query_biome_color_hex(biome) + "\", " +
                sprintf("%.1f", n["height"]) + ", " +
                "\"" + query_humidity_color_hex(n["humidity"]) + "\", " +
                "\"" + query_heat_color_hex(n["heat"]) + "\" ]";

            if (x < size-1) line += ",";
        }
        line += "]" + (y == size - 1 ? "" : ",");
        write_file("/tmp/"+name+".json", line + "\n");
    }
    write_file("/tmp/"+name+".json", "    ],\n    \"height_min\":\""+height_min+"\",\n    \"height_max\":\""+height_max+"\",\n    \"humidity_min\":\""+humidity_min+"\",\n    \"humidity_max\":\""+humidity_max+"\",\n    \"heat_min\":\""+heat_min+"\",\n    \"heat_max\":\""+heat_max+"\"\n}");
    write("Seed '"+name+"' size "+size+" done\n");
    foreach (string key,int value in biomes) {
        write(sprintf("%20s : %10s", key, format_integer(value)) + " : " + sprintf("%2.2f", value * 100.0 / (size * size)) + "%\n");
    }
    write(sprintf("%-24s", "height min: "+height_min)+" "+sprintf("%-24s", "height_max: "+height_max)+"\n");
    write(sprintf("%-24s", "humidity min: "+humidity_min)+" "+sprintf("%-24s", "humidity_max: "+humidity_max)+"\n");
    write(sprintf("%-24s", "heat min: "+heat_min)+" "+sprintf("%-24s", "heat_max: "+heat_max)+"\n");
}

void generate_simplex_json_resource (string name) {
    int x, y, size, size2, size9_10;
    mapping p;
    string line;
    float nResource;
    float min = 1, max = -1;
    float tZW = time() / 86400 % 100 / 100.0; // changes every 24 hours [0.00-0.99]

    // size = query_planet_size(name);
    size = 500;
    size2 = size/2;
    size9_10 = size2 * 9 / 10;

    p = noise_generate_permutation_simplex(name);

    write_file("/tmp/"+name+".json", "{\n    \"name\":\""+name+"\",\n    \"size\":\""+size+"\",\n    \"data\":[\n", 1);
    for (y = 0; y < size; y ++) {
        line = "        [ ";
        for (x = 0; x < size; x ++) {

            nResource = (noise_simplex_4d(x*1.0/size, y*1.0/size, tZW, tZW, p, 4, 10.0, 3.0) + 1) / 2;
            nResource = floor(nResource * 100.0);
            if (nResource < min) min = nResource;
            if (nResource > max) max = nResource;

            if (to_int(nResource) % 10 == 0) {
                nResource = 1.0;
            } else {
                nResource = 0.0;
            }

            line += sprintf("%.2f", nResource);

            if (x < size-1) line += ",";
        }
        line += "]" + (y == size - 1 ? "" : ",");
        write_file("/tmp/"+name+".json", line + "\n");
    }
    write_file("/tmp/"+name+".json", "    ],\n    \"min\":\""+min+"\",\n    \"max\":\""+max+"\"\n}");
    write("Seed '"+name+"' size "+size+" done\n");
}