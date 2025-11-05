#include "planet.h"

inherit M_CLEAN;

#define PI                  3.141592653589793
#define PIx2                6.283185307179586

#define WATER_LAKES         0.75
#define WATER_RIVER_1       0.60
#define WATER_RIVER_2       0.40

#define LEVEL_RANGE 20

private mapping __Planet = ([
/*
    Data Format:
    "name": string,
    "size": integer,

    "heightFactor": 1.0,
    "humidityFactor": 1.0,
    "heatFactor": 1.0,

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

string *query_all_planets () {
    mixed *results, *subresults;
    string *planets = ({ });
    results = filter(get_dir("/save/planet/", -1), (: $1[1] == -2 :));
    foreach (string dir in results) {
        subresults = get_dir("/save/planet/" + dir[0] + "/*.o");
        foreach (mixed *file in subresults) {
            planets += ({ file[0..<3] });
        }
    }
    return planets;
}

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
    return !!save_object(path);
}

int adjust_planet (string name, mapping config) {
    string path = "/save/planet/" + lower_case(name[0..0]) + "/" + name + ".o";
    if (file_size(path) == -1) {
        return 0;
    }
    __Planet = ([ ]);
    map_delete(config, "name");
    restore_object(path);
    __Planet += config;
    return !!save_object(path);
}

/* ----- noise ----- */

varargs mapping query_noise (mapping p, int size, int x, int y, float heightFactor, float humidityFactor, float heatFactor, int time) {
    int size2, level;
    float nx, ny, nz, nw, now, nowAdj;
    float nHeight, nHumidity, nHeat, nTmp, nResource = -1.0;

    if (undefinedp(heightFactor)) {
        heightFactor = 1.0;
    }
    if (undefinedp(humidityFactor)) {
        humidityFactor = 1.0;
    }
    if (undefinedp(heatFactor)) {
        heatFactor = 1.0;
    }
    if (undefinedp(time)) {
        time = time();
    }

    // Calculate our 4D coordinates
    nx = nz = to_float(x);
    ny = nw = to_float(y);
    nx = cos((nx / size) * PIx2) * 2 / PIx2;
    ny = cos((ny / size) * PIx2) * 2 / PIx2;
    nz = sin((nz / size) * PIx2) * 2 / PIx2;
    nw = sin((nw / size) * PIx2) * 2 / PIx2;
    now = nowAdj = time / 86400 % 100 / 100.0; // changes every 24 hours, 0.00-0.99
    if (nowAdj > 0.5) {
        nowAdj -= 0.5;
    }

    // level
    size2 = size / 2;
    level = to_int(sqrt((size2-x) * (size2-x) + (size2-y) * (size2-y)) / (size2 * 1.0 / LEVEL_RANGE));
    level = max(({ 1, min(({ LEVEL_RANGE, level })) }));

    if (heatFactor != 0.0) {
        // noise Humidity
        nHumidity = max(({ 0.0, (((noise_simplex_4d(nx + nowAdj, ny + nowAdj, nz + nowAdj, nw + nowAdj, p, 4, 3.0) + 1) / 2) - 0.25) / 0.5 })); // normalize 0.25-0.75 to 0-1
        nHumidity *= humidityFactor;
    }

    // noise Height
    nHeight = (((noise_simplex_4d(nx, ny, nz, nw, p, 5, 1.25) + 1) / 2) - 0.25) / 0.5; // normalize 0.25-0.75 to 0-1
    nHeight *= heightFactor;
    // ensure central land mass exists
    if (
        (nHeight <= HEIGHT_SHALLOW) &&
        (x <= size2+3 && y <= size2+3 && x >= size2-3 && y >= size2-3) &&
        (sqrt((size2-x) * (size2-x) + (size2-y) * (size2-y)) + 0.5 < 3)
    ) {
        nHeight += (HEIGHT_SHALLOW - nHeight) + 0.05;
    } else {
        // noise River
        nTmp = (((noise_simplex_4d(nx, ny, nz, nw, p, 4, 1.25) + 1) / 2) - 0.25) / 0.5; // normalize 0.25-0.75 to 0-1
        if (nTmp >= WATER_LAKES-0.026 && nTmp <= WATER_LAKES+0.026) {
            if (nTmp >= WATER_LAKES-0.025 && nTmp <= WATER_LAKES+0.025) {
                nHeight = max(({ 0.0, nHeight - (abs(nHeight - HEIGHT_SHALLOW) + 0.05) }));
            }
            if (heatFactor != 0.0) {
                nHumidity += 0.1 * nHeight;
            }
        } else {
            nTmp = (((noise_simplex_4d(nw, nz, ny, nx, p, 5, 1.25) + 1) / 2) - 0.25) / 0.5; // normalize 0.25-0.75 to 0-1
            if (
                (nTmp >= WATER_RIVER_1-0.026 && nTmp <= WATER_RIVER_1+0.026) ||
                (nTmp >= WATER_RIVER_2-0.026 && nTmp <= WATER_RIVER_1+0.026)
            ) {
                if (
                    (nTmp >= WATER_RIVER_1-0.025 && nTmp <= WATER_RIVER_1+0.025) ||
                    (nTmp >= WATER_RIVER_2-0.025 && nTmp <= WATER_RIVER_2+0.025)
                ) {
                    if (nHeight <= HEIGHT_SHALLOW) {
                        nHeight = nHeight * 0.8;
                    } else {
                        nHeight = max(({ 0.0, nHeight - (abs(nHeight - HEIGHT_SHALLOW) + 0.05) }));
                    }
                }
                if (heatFactor != 0.0) {
                    nHumidity += 0.1 * nHeight;
                }
            }
        }
    }

    if (heatFactor != 0.0) {
        // adjust noise Humidity based upon noise Height, lower is wetter
        if (nHeight <= HEIGHT_DEEPER) {
            nHumidity += 5.0 * nHeight;
        } else if (nHeight <= HEIGHT_DEEP) {
            nHumidity += 4.0 * nHeight;
        } else if (nHeight <= HEIGHT_SHALLOW) {
            nHumidity += 3.0 * nHeight;
        } else if (nHeight <= HEIGHT_SHORE) {
            nHumidity += 0.2 * nHeight;
        }
    }

    if (heatFactor != 0.0) {
        // noise Heat
        nHeat = abs((noise_simplex_4d(nx + nowAdj, ny + nowAdj, nz + nowAdj, nw + nowAdj, p, 4, 2.5) + 1) / 2);
        nHeat = abs((nHeat - 0.05) / (0.95 - 0.05));
        // noise Gradient
        size2 = size * 45 / 100;
        if (y >= size2 && y <= size - size2) {  // center 20%
            nTmp = 1.0;
        } else if (y < size2) {                 // north 40%
            nTmp = 1.0 - (gradient_2d(1.0, 1.0, 1.0, 0.0, 0.0, (y * 1.0) / size2));
        } else {                                // south 40%
            nTmp = 1.0 - (gradient_2d(1.0, 1.0, 1.0, 0.0, 0.0, (size - (y + 1.0)) / size2));
        }
        nHeat = nHeat * nTmp * 1.5;
        nHeat *= heatFactor;
        if (nHeat > 1.0) {
            nHeat = 1.0;
        }
        // adjust noise Heat based upon noise Height, higher is colder
        if (nHeight > HEIGHT_SHALLOW) {
            nHeat -= (nHeight - HEIGHT_SHALLOW) * nHeight;
        }
        nHeat = max(({ nHeat, 0.0 }));
    }

    // noise Resource
    if (nHeight > HEIGHT_SHORE) {
        nResource = ((noise_simplex_4d(nx + now, ny + now, nz + now, nw + now, p, 4, 25.0) + 1) / 2);
    }

    return ([
        "level": level,
        "height": nHeight,
        "humidity": nHumidity,
        "heat": nHeat,
        "resource": to_int(nResource * 100.0) % 10,
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
    } else {
        // HEAT_HOTTEST
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

nosave private mapping __BiomeColor = ([
    "deeper water":         ([
        "ANSI": "\e[38;2;0;0;96m",
        "RGB": "#000060",
    ]),
    "deep water":           ([
        "ANSI": "\e[38;2;0;0;128m",
        "RGB": "#000080",
    ]),
    "shallow water":        ([
        "ANSI": "\e[38;2;25;25;150m",
        "RGB": "#191996",
    ]),
    "icy water":            ([
        "ANSI": "\e[38;2;105;189;230m",
        "RGB": "#69BDE6",
    ]),
    "frozen water":         ([
        "ANSI": "\e[38;2;189;219;246m",
        "RGB": "#BDDBF6",
    ]),
    "ice":                  ([
        "ANSI": "\e[38;2;255;255;255m",
        "RGB": "#FFFFFF",
    ]),
    "tundra":               ([
        "ANSI": "\e[38;2;96;131;112m",
        "RGB": "#608370",
    ]),
    "grassland":            ([
        "ANSI": "\e[38;2;164;255;99m",
        "RGB": "#A4FF63",
    ]),
    "woodland":             ([
        "ANSI": "\e[38;2;139;175;90m",
        "RGB": "#8BAF5A",
    ]),
    "boreal forest": ([
        "ANSI": "\e[38;2;95;115;62m",
        "RGB": "#5F733E",
    ]),
    "desert":  ([
        "ANSI": "\e[38;2;238;218;130m",
        "RGB": "#EEDA82",
    ]),
    "temperate rainforest": ([
        "ANSI": "\e[38;2;29;73;40m",
        "RGB": "#1D4928",
    ]),
    "savanna": ([
        "ANSI": "\e[38;2;177;209;110m",
        "RGB": "#B1D16E",
    ]),
    "tropical rainforest": ([
        "ANSI": "\e[38;2;66;123;25m",
        "RGB": "#427B19",
    ]),
    "default": ([
        "ANSI": "\e[38;2;128;0;0m",
        "RGB": "#800000",
    ]),
]);
string query_biome_color_ansi (string biome) {
    if (biome && __BiomeColor[biome]) {
        return __BiomeColor[biome]["ANSI"];
    } else {
        return __BiomeColor["default"]["ANSI"];
    }
}
string query_biome_color_hex (string biome) {
    if (biome && __BiomeColor[biome]) {
        return __BiomeColor[biome]["RGB"];
    } else {
        return __BiomeColor["default"]["RGB"];
    }
}
nosave private mapping __HumidityColorHex = ([
    ""+HUMIDITY_WETTEST:        "#000064", // 0;0;100
    ""+HUMIDITY_WETTER:         "#1446FF", // 20;70;255
    ""+HUMIDITY_WET:            "#55FFFF", // 85;255;255
    ""+HUMIDITY_DRY:            "#50FF00", // 80;255;0
    ""+HUMIDITY_DRYER:          "#50FF00", // 245;245;17
    ""+HUMIDITY_DRYEST:         "#FF8B11", // 255;139;17
    "default":                  "#000000", // 0;0;0
]);
string query_humidity_color_hex (float humidity) {
    if (!undefinedp(humidity)) {
        foreach (string key in sort_array(keys(__HumidityColorHex), 1)) {
            if (humidity <= to_float(key)) {
                return __HumidityColorHex[key];
            }
        }
    }
    return __HumidityColorHex["default"];
}
nosave private mapping __HeatColorHex = ([
    ""+HEAT_HOTTEST:            "#F10C00", // 241;12;0
    ""+HEAT_HOTTER:             "#FF6400", // 255;100;0
    ""+HEAT_HOT:                "#FFFF64", // 255;255;100
    ""+HEAT_COLD:               "#00E585", // 0;229;133
    ""+HEAT_COLDER:             "#AAFFFF", // 170;255;255
    ""+HEAT_COLDEST:            "#00FFFF", // 0;255;255
    "default":                  "#000000", // 0;0;0
]);
string query_heat_color_hex (float heat) {
    if (!undefinedp(heat)) {
        foreach (string key in sort_array(keys(__HeatColorHex), 1)) {
            if (heat <= to_float(key)) {
                return __HeatColorHex[key];
            }
        }
    }
    return __HeatColorHex["default"];
}
nosave private mapping __ResourceColorHex = ([
    "2":                        "#A47449", // 164, 116, 73
    "1":                        "#C0C0C0", // 192, 192, 192
    "default":                  "#000000",
]);
string query_resource_color_hex (int resource) {
    if (resource) {
        foreach (string key in sort_array(keys(__ResourceColorHex), 1)) {
            if (resource <= to_int(key)) {
                return __ResourceColorHex[key];
            }
        }
    }
    return __ResourceColorHex["default"];
}

/* ----- export /tmp/name.json ----- */

void generate_json_line (mapping data, int time) {
    mapping p = data["p"], planet = data["planet"];
    int y = data["y"];
    string biome, line = "        [ ";
    mapping n;
    int size = planet["size"];

    for (int x = 0; x < size; x ++) {
        n = query_noise(p, size, x, y, planet["heightFactor"], planet["humidityFactor"], planet["heatFactor"], time);
        if (n["height"] < data["min"]["height"]) {
            data["min"]["height"] = n["height"];
        } else if (n["height"] > data["max"]["height"]) {
            data["max"]["height"] = n["height"];
        }
        if (n["humidity"] < data["min"]["humidity"]) {
            data["min"]["humidity"] = n["humidity"];
        } else if (n["humidity"] > data["max"]["humidity"]) {
            data["max"]["humidity"] = n["humidity"];
        }
        if (n["heat"] < data["min"]["heat"]) {
            data["min"]["heat"] = n["heat"];
        } else if (n["heat"] > data["max"]["heat"]) {
            data["max"]["heat"] = n["heat"];
        }
        biome = query_biome(n["height"], n["heat"], n["humidity"]);
        data["biomes"][biome] ++;
        data["levels"][n["level"]] ++;
        data["resources"][n["resource"]] ++;
        line += "[" +
            "\"" + query_biome_color_hex(biome) + "\"," +
            sprintf("%.2f", floor(n["height"]*20)/20.0) + "," +
            "\"" + query_humidity_color_hex(n["humidity"]) + "\"," +
            "\"" + query_heat_color_hex(n["heat"]) + "\"," +
            sprintf("%.2f", (n["level"] * 1.0 / LEVEL_RANGE)) + "," +
            "\"" + query_resource_color_hex(n["resource"]) + "\"" +
            "]";

        if (x + 1 < size) {
            line += ",";
        }
    }
    line += "]" + (y == size - 1 ? "" : ",");
    write_file(data["file"], line + "\n");

    if (y + 1 == size) {
        // int size2 = size * size;
        int t;
        write_file(data["file"], "]}");
        // write_file(data["file"], "    ],\n    \"min height\":\""+data["min"]["height"]+"\",\n    \"max height\":\""+data["max"]["height"]+"\",\n    \"min humidity\":\""+data["min"]["humidity"]+"\",\n    \"max humidity\":\""+data["max"]["humidity"]+"\",\n    \"min heat\":\""+data["min"]["heat"]+"\",\n    \"max heat\":\""+data["max"]["heat"]+"\"\n}");

        t = time_ns() - data["start"];
        // write("Seed '"+planet["name"]+"' size "+size+" "+data["file"]+" done: %^ORANGE%^"+sprintf("%.2f", t/1000000.0)+" ms%^RESET%^\n");

        // TODO: clean this up
        // foreach (string b in keys(__BiomeColor)) {
        //     write(sprintf("%20s : %10s", b, format_integer(data["biomes"][b])) + " : " + sprintf("%2.2f", data["biomes"][b] * 100.0 / size2) + "%\n");
        // }
        // write(sprintf("%-24s", "min height: "+data["min"]["height"])+" "+sprintf("%-24s", "max height: "+data["max"]["height"])+"\n");
        // write(sprintf("%-24s", "min humidity: "+data["min"]["humidity"])+" "+sprintf("%-24s", "max humidity: "+data["max"]["humidity"])+"\n");
        // write(sprintf("%-24s", "min heat: "+data["min"]["heat"])+" "+sprintf("%-24s", "max heat: "+data["max"]["heat"])+"\n");
        // write(
        //     sprintf(
        //         "%-24s",
        //         "land: "+(data["biomes"]["ice"]+data["biomes"]["tundra"]+data["biomes"]["grassland"]+data["biomes"]["woodland"]+data["biomes"]["boreal forest"]+data["biomes"]["desert"]+data["biomes"]["temperate rainforest"]+data["biomes"]["savanna"]+data["biomes"]["tropical rainforest"])*100.0/(size*size)
        //     )+" "+
        //     sprintf(
        //         "%-24s",
        //         "water: "+(data["biomes"]["shallow water"]+data["biomes"]["deep water"]+data["biomes"]["deeper water"]+data["biomes"]["icy water"]+data["biomes"]["frozen water"])*100.0/(size*size)
        //     )+"\n"
        // );
        // write("level distribution:\n");
        // foreach (int key,int value in data["levels"]) {
        //     write(sprintf("%20s : %10s", ""+key, format_integer(value)) + " : " + sprintf("%2.2f", value * 100.0 / (size * size)) + "%\n");
        // }
        // write("resource distribution:\n");
        // foreach (int key,int value in data["resources"]) {
        //     write(sprintf("%20s : %10s", ""+key, format_integer(value)) + " : " + sprintf("%2.2f", value * 100.0 / (size * size)) + "%\n");
        // }
    } else {
        data["y"] ++;
        if (data["y"] % 5 > 0) {
            generate_json_line(data, time);
        } else {
            call_out_walltime((: generate_json_line :), 0.001, data, time);
        }
    }
}
varargs void generate_json (string name, int time) {
    mapping data = ([
        "start": time_ns(),
        "name": name,
        "min": ([
            "height": 1,
            "humidity": 1,
            "heat": 1,
        ]),
        "max": ([
            "height": -1,
            "humidity": -1,
            "heat": -1,
        ]),
        "biomes": ([ ]),
        "levels": ([ ]),
        "resources": ([ ]),
        "planet": query_planet(name),
        "p": noise_generate_permutation_simplex(name),
        "y": 0,
        "file": "/tmp/"+name+".json",
    ]);
    if (undefinedp(time)) {
        time = time();
    }

    write_file(data["file"], "{\n\"name\":\""+name+"\",\n\"size\":\""+data["planet"]["size"]+"\",\n\"data\":[\n", 1);

    call_out_walltime((: generate_json_line :), 0.001, data, time);
}