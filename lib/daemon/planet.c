inherit M_CLEAN;

private mapping __Planet = ([ ]);

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

int query_planet_terrain (string name, int x, int y, int size) {
    float terrain, nx, ny;
    int *p;

    nx = to_float(x) / size;
    ny = to_float(y) / size;
    p = perlin_generate_permutation(name);
    terrain = (1.0 + perlin_noise_2d(nx, ny, p, 8, 15.0)) * 0.5;

    terrain = terrain * 12;

    return to_int(terrain);
}

/* ----- map ----- */

string *query_planet_map (string name, int x, int y, int radius) {
    string *msg = ({ }), symbol;
    int xx, yy, size;
    float distance;
    float terrain, nx, ny;
    int *p;

    size = query_planet_size(name);
    p = perlin_generate_permutation(name);

    for (yy = -radius; yy <= radius; yy++) {
        for (xx = -radius; xx <= radius; xx++) {
            distance = sqrt(xx * xx + yy * yy) - 0.1;
            if (distance > radius) {
                msg += ({ "[ ]" });
                continue;
            }

            // @TODO wrap

            if (xx == 0 && yy == 0) {
                symbol = "%^CYAN%^BOLD%^X%^RESET%^";
            } else {
                symbol = " ";
            }

            nx = to_float(x + xx) / size;
            ny = to_float(y + yy) / size;
            terrain = (1.0 + perlin_noise_2d(nx, ny, p, 8, 15.0)) * 0.5;

            switch(to_int(terrain * 12)) {
            case 0..2:
                msg += ({ "%^BLUE%^["+symbol+"%^BLUE%^]%^RESET%^" });
                break;
            case 3..4:
                msg += ({ "%^BLUE%^BOLD%^["+symbol+"%^BLUE%^BOLD%^]%^RESET%^" });
                break;
            case 5:
                msg += ({ "%^YELLOW%^["+symbol+"%^YELLOW%^]%^RESET%^" });
                break;
            case 6:
                msg += ({ "%^GREEN%^BOLD%^["+symbol+"%^GREEN%^BOLD%^]%^RESET%^" });
                break;
            case 7:
                msg += ({ "%^GREEN%^["+symbol+"%^GREEN%^]%^RESET%^" });
                break;
            case 8..10:
                msg += ({ "%^ORANGE%^["+symbol+"%^ORANGE%^]%^RESET%^" });
                break;
            case 11..12:
                msg += ({ "%^BOLD%^["+symbol+"%^BOLD%^]%^RESET%^" });
                break;
            default:
                msg += ({ "%^RED%^["+symbol+"%^RED%^]%^RESET%^" });
                break;
            }
        }
        msg += ({ "\n" });
    }

    return msg;
}