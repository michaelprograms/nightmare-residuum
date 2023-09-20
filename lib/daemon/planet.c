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
    restore_object(path);
    return __Planet;
}

int query_planet_size (string name) {
    string path = "/save/planet/" + lower_case(name[0..0]) + "/" + name + ".o";
    restore_object(path);
    return __Planet["size"] ? __Planet["size"] : 100;
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
    if (file_size(path) > 0) {
        return 0;
    }
    map_delete(config, "name");
    restore_object(path);
    __Planet += config;
    return save_object(path);
}

/* ----- terrain ----- */

#define PI 3.141592653589793

float query_planet_surface_terrain (string name, int x, int y, int size) {
    float nx, ny, nz, nw, dx, dy, t;
    int x1, x2, y1, y2;
    mapping p = noise_generate_permutation_simplex(name);

    x1 = 0;
    x2 = 2;
    y1 = 0;
    y2 = 2;
    dx = x2 - x1;
    dy = y2 - y1;

    nx = to_float(x);
    ny = to_float(y);
    nz = nx;
    nw = ny;
    nx = x1 + cos((nx / size) * 2 * PI) * dx / (2 * PI);
    ny = y1 + cos((ny / size) * 2 * PI) * dy / (2 * PI);
    nz = x1 + sin((nz / size) * 2 * PI) * dx / (2 * PI);
    nw = y1 + sin((nw / size) * 2 * PI) * dy / (2 * PI);
    t = (noise_simplex_4d(nx, ny, nz, nw, p, 8, 3.0) + 1) / 2;

    return t;
}


float query_planet_terrain_by_p (int *p, int x, int y, int size) {
    float nx, ny;
    float terrain;

    nx = x / (size - 1.0);
    ny = y / (size - 1.0);

    terrain = (1.0 + noise_perlin_2d(nx, ny, p, 8, 15.0)) * 0.5;
    // terrain = (1.0 + noise_perlin_3d(nx, ny, 0.0, p, 8, 15.0)) * 0.5;

    return terrain;
}
float query_planet_terrain (string name, int x, int y, int size) {
    int *p = noise_generate_permutation(name);
    return query_planet_terrain_by_p(p, x, y, size);
}

/* ----- map ----- */

string *query_planet_map (string name, int x, int y, int radius, int fog) {
    string *msg = ({ }), symbol;
    int xx, yy, size;
    float distance, terrain;
    int *p;

    size = query_planet_size(name);
    p = noise_generate_permutation(name);

    for (yy = -radius; yy <= radius; yy++) {
        for (xx = -radius; xx <= radius; xx++) {
            distance = sqrt(xx * xx + yy * yy) - 0.1;
            if (!fog && distance > radius) {
                msg += ({ "[ ]" });
                continue;
            }

            if (xx == 0 && yy == 0) {
                symbol = "%^CYAN%^BOLD%^X%^RESET%^";
            } else {
                symbol = " ";
            }

            terrain = query_planet_terrain_by_p(p, x + xx, y + yy, size);
            if (terrain <= 0.2) {
                msg += ({ "\e[38;5;4m[" + symbol + "\e[38;5;4m]%^RESET%^" });     // BLUE foreground
            } else if (terrain <= 0.25) {
                msg += ({ "\e[38;5;12m[" + symbol + "\e[38;5;12m]%^RESET%^" });    // BOLD BLUE foreground
            } else if (terrain <= 0.3) {
                msg += ({ "\e[38;5;6m[" + symbol + "\e[38;5;6m]%^RESET%^" });     // CYAN foreground
            } else if (terrain <= 0.4) {
                msg += ({ "\e[38;5;14m[" + symbol + "\e[38;5;14m]%^RESET%^" });    // BOLD CYAN foreground
            } else if (terrain <= 0.5) {
                msg += ({ "\e[38;5;11m[" + symbol + "\e[38;5;11m]%^RESET%^" });    // YELLOW foreground
            } else if (terrain <= 0.6) {
                msg += ({ "\e[38;5;10m[" + symbol + "\e[38;5;10m]%^RESET%^" });    // BOLD GREEN foreground
            } else if (terrain <= 0.7) {
                msg += ({ "\e[38;5;2m[" + symbol + "\e[38;5;2m]%^RESET%^" });     // GREEN foreground
            } else if (terrain <= 0.8) {
                msg += ({ "\e[38;5;3m[" + symbol + "\e[38;5;3m]%^RESET%^" });     // ORANGE foreground
            } else if (terrain <= 0.9) {
                msg += ({ "\e[38;5;7m[" + symbol + "\e[38;5;7m]%^RESET%^" });     // WHITE foreground
            } else if (terrain <= 1.0) {
                msg += ({ "\e[38;5;15m[" + symbol + "\e[38;5;15m]%^RESET%^" });    // BOLD WHITE foreground
            } else {
                msg += ({ "\e[38;5;9m[" + symbol + "\e[38;5;9m]%^RESET%^" });
            }
        }
        msg += ({ "\n" });
    }

    return msg;
}

string *query_planet_pixel_map (string name, int x, int y, int radius) {
    string *msg = ({ }), pixelTop, pixelBot;
    int xx, yy, size, rx, ry;
    float terrain;
    int *p;
    // string line;
    // mapping perm;
    // float nx, ny, nz, nw, dx, dy;
    // int x1, x2, y1, y2;


    size = query_planet_size(name);
    // if (version == 4) {
    //     perm = noise_generate_permutation_simplex(name);
    //     x1 = 0;
    //     x2 = 2;
    //     y1 = 0;
    //     y2 = 2;
    //     dx = x2 - x1;
    //     dy = y2 - y1;
    // } else {
        p = noise_generate_permutation(name);
    // }

    for (yy = -radius; yy <= radius; yy += 2) {
        // line = "";
        for (xx = -radius; xx <= radius; xx ++) {
            if (x + xx < 0) rx = size + (x + xx);
            else rx = x + xx;
            if (y + yy < 0) ry = size + (y + yy);
            else ry = y + yy;
            // top pixel
            // if (version == 4) {
            //     // Calculate our 4D coordinates
            //     nx = to_float(rx);
            //     ny = to_float(ry);
            //     nz = nx;
            //     nw = ny;
            //     nx = x1 + cos((nx / size) * 2 * PI) * dx / (2 * PI);
            //     ny = y1 + cos((ny / size) * 2 * PI) * dy / (2 * PI);
            //     nz = x1 + sin((nz / size) * 2 * PI) * dx / (2 * PI);
            //     nw = y1 + sin((nw / size) * 2 * PI) * dy / (2 * PI);
            //     terrain = (noise_simplex_4d(nx, ny, nz, nw, perm, 8, 3.0) + 1) / 2;
            // } else {
                terrain = query_planet_terrain_by_p(p, rx, ry, size);
            // }
            if (terrain <= 0.2) {
                pixelTop = "\e[48;5;4";     // BLUE background
            } else if (terrain <= 0.25) {
                pixelTop = "\e[48;5;12";    // BOLD BLUE background
            } else if (terrain <= 0.3) {
                pixelTop = "\e[48;5;6";     // CYAN background
            } else if (terrain <= 0.4) {
                pixelTop = "\e[48;5;14";    // BOLD CYAN background
            } else if (terrain <= 0.5) {
                pixelTop = "\e[48;5;11";    // YELLOW background
            } else if (terrain <= 0.6) {
                pixelTop = "\e[48;5;10";    // BOLD GREEN background
            } else if (terrain <= 0.7) {
                pixelTop = "\e[48;5;2";     // GREEN background
            } else if (terrain <= 0.8) {
                pixelTop = "\e[48;5;3";     // ORANGE background
            } else if (terrain <= 0.9) {
                pixelTop = "\e[48;5;7";     // WHITE background
            } else if (terrain <= 1.0) {
                pixelTop = "\e[48;5;15";    // BOLD WHITE background
            } else {
                pixelTop = "\e[48;5;9";
            }

            // bottom pixel
            if (x + xx < 0) rx = size + (x + xx);
            else rx = x + xx;
            if (y + yy + 1 < 0) ry = size + (y + yy + 1);
            else ry = y + yy + 1;
            // if (version == 4) {
            //     // Calculate our 4D coordinates
            //     nx = to_float(rx);
            //     ny = to_float(ry);
            //     nz = nx;
            //     nw = ny;
            //     nx = x1 + cos((nx / size) * 2 * PI) * dx / (2 * PI);
            //     ny = y1 + cos((ny / size) * 2 * PI) * dy / (2 * PI);
            //     nz = x1 + sin((nz / size) * 2 * PI) * dx / (2 * PI);
            //     nw = y1 + sin((nw / size) * 2 * PI) * dy / (2 * PI);
            //     terrain = (noise_simplex_4d(nx, ny, nz, nw, perm, 8, 3.0) + 1) / 2;
            // } else {
                terrain = query_planet_terrain_by_p(p, rx, ry, size);
            // }
            if (terrain <= 0.2) {
                pixelBot = ";38;5;4";     // BLUE foreground
            } else if (terrain <= 0.25) {
                pixelBot = ";38;5;12";    // BOLD BLUE foreground
            } else if (terrain <= 0.3) {
                pixelBot = ";38;5;6";     // CYAN foreground
            } else if (terrain <= 0.4) {
                pixelBot = ";38;5;14";    // BOLD CYAN foreground
            } else if (terrain <= 0.5) {
                pixelBot = ";38;5;11";    // YELLOW foreground
            } else if (terrain <= 0.6) {
                pixelBot = ";38;5;10";    // BOLD GREEN foreground
            } else if (terrain <= 0.7) {
                pixelBot = ";38;5;2";     // GREEN foreground
            } else if (terrain <= 0.8) {
                pixelBot = ";38;5;3";     // ORANGE foreground
            } else if (terrain <= 0.9) {
                pixelBot = ";38;5;7";     // WHITE foreground
            } else if (terrain <= 1.0) {
                pixelBot = ";38;5;15";    // BOLD WHITE foreground
            } else {
                pixelBot = ";38;5;9";
            }
            // line += pixelTop + pixelBot + "▄\e[0m";
            write(pixelTop + pixelBot + "m▄\e[0m");
        }
        // msg += ({ line });
        write("\n");
    }
    return msg;
}

void generate_simplex_json (string name) {
    int x, y, size;
    mapping p;
    string line;
    float nx, ny, nz, nw, dx, dy, t;
    int x1, x2, y1, y2;

    size = query_planet_size(name);
    p = noise_generate_permutation_simplex(name);

    x1 = 0;
    x2 = 2;
    y1 = 0;
    y2 = 2;
    dx = x2 - x1;
    dy = y2 - y1;

    write_file("/tmp/data.json", "{\n    \"name\":\""+name+"\",\n    \"size\":\""+size+"\",\n    \"data\":[\n", 1);
    for (y = 0; y < size; y ++) {
        line = "        [ ";
        for (x = 0; x < size; x ++) {
            // Calculate our 4D coordinates
            nx = to_float(x);
            ny = to_float(y);
            nz = nx;
            nw = ny;
            nx = x1 + cos((nx / size) * 2 * PI) * dx / (2 * PI);
            ny = y1 + cos((ny / size) * 2 * PI) * dy / (2 * PI);
            nz = x1 + sin((nz / size) * 2 * PI) * dx / (2 * PI);
            nw = y1 + sin((nw / size) * 2 * PI) * dy / (2 * PI);
            t = (noise_simplex_4d(nx, ny, nz, nw, p, 8, 3.0) + 1) / 2;

            if (t <= 0.2) {
                line += "\"#000080\"";          // BLUE
            } else if (t <= 0.25) {
                line += "\"#0000FF\"";          // BOLD BLUE
            } else if (t <= 0.3) {
                line += "\"#008080\"";          // CYAN
            } else if (t <= 0.4) {
                line += "\"#00FFFF\"";          // BOLD CYAN
            } else if (t <= 0.5) {
                line += "\"#FFFF00\"";          // YELLOW
            } else if (t <= 0.6) {
                line += "\"#00FF00\"";          // BOLD GREEN
            } else if (t <= 0.7) {
                line += "\"#008000\"";          // GREEN
            } else if (t <= 0.8) {
                line += "\"#808000\"";          // ORANGE
            } else if (t <= 0.9) {
                line += "\"#C0C0C0\"";          // WHITE
            } else if (t <= 1.0) {
                line += "\"#FFFFFF\"";          // BOLD WHITE
            } else {
                line += "\"#800000\"";
            }
            if (x < size-1) line += ",";

        }
        line += "]"+(y==size-1?"":",");
        write_file("/tmp/data.json", line + "\n");
    }
    write_file("/tmp/data.json", "    ]\n}");
    write(name+" "+size+" done\n");
}

void generate_perlin_json (string name) {
    int x, y, size;
    int *p;
    string line;
    float t;

    size = query_planet_size(name);
    p = noise_generate_permutation(name);

    write_file("/tmp/data.json", "{\n    \"name\":\""+name+"\",\n    \"size\":\""+size+"\",\n    \"data\":[\n", 1);
    for (y = 0; y < size; y ++) {
        line = "        [ ";
        for (x = 0; x < size; x ++) {
            t = query_planet_terrain_by_p(p, x, y, size);
            if (t <= 0.2) {
                line += "\"#000080\"";          // BLUE
            } else if (t <= 0.25) {
                line += "\"#0000FF\"";          // BOLD BLUE
            } else if (t <= 0.3) {
                line += "\"#008080\"";          // CYAN
            } else if (t <= 0.4) {
                line += "\"#00FFFF\"";          // BOLD CYAN
            } else if (t <= 0.5) {
                line += "\"#FFFF00\"";          // YELLOW
            } else if (t <= 0.6) {
                line += "\"#00FF00\"";          // BOLD GREEN
            } else if (t <= 0.7) {
                line += "\"#008000\"";          // GREEN
            } else if (t <= 0.8) {
                line += "\"#808000\"";          // ORANGE
            } else if (t <= 0.9) {
                line += "\"#C0C0C0\"";          // WHITE
            } else if (t <= 1.0) {
                line += "\"#FFFFFF\"";          // BOLD WHITE
            } else {
                line += "\"#800000\"";
            }
            if (x < size-1) line += ",";

        }
        line += "]"+(y==size-1?"":",");
        write_file("/tmp/data.json", line + "\n");
    }
    write_file("/tmp/data.json", "    ]\n}");
    write(name+" "+size+" done\n");
}