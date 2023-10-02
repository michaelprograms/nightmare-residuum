#include <planet.h>

inherit STD_ROOM;

int is_virtual_room () { return 1; }

void create() {
    ::create();
    set_short("a terrain somewhere");
    set_long("The terrain of a planet.");
}

/* ----- biome ----- */

void set_biome (mapping n) {
    string biome = D_PLANET->query_biome(n["height"], n["heat"], n["humidity"]);
    string color = D_PLANET->query_biome_color_ansi(biome);

    // set_long_footer("height="+height+" heat="+heat+" humidity="+humidity);

    set_room_square_color(color);
    set_short(biome);

    switch (biome) {
    case "icy water":
        set_long("Surrounded by deeper water.");
        break;
    case "deeper water":
        set_long("Surrounded by deeper water.");
        break;
    case "deep water":
        set_long("Surrounded by deep water.");
        break;
    case "shallow water":
        set_long("Surrounded by water.");
        break;
    case "ice":
        set_long("Surrounded by ice.");
        break;
    case "tundra":
        set_long("Surrounded by tundra.");
        break;
    case "grassland":
        set_long("Surrounded by grassland.");
        break;
    case "woodland":
        set_long("Surrounded by woodland.");
        break;
    case "boreal forest":
        set_long("Surrounded by boreal forest.");
        break;
    case "desert":
        set_long("Surrounded by desert.");
        break;
    case "woodland":
        set_long("Surrounded by woodland.");
        break;
    case "seasonal forest":
        set_long("Surrounded by seasonal forest.");
        break;
    case "temperate rainforest":
        set_long("Surrounded by temperate rainforest.");
        break;
    case "savanna":
        set_long("Surrounded by savanna.");
        break;
    case "tropical rainforest":
        set_long("Surrounded by tropical rainforest.");
        break;
    default:
        set_long("Error: unknown biome.");
        break;
    }
}

void add_terrain_override (string text) {
    set_long(query_long() + " " + text);
}

/* ----- map override ----- */

string query_room_square_color () {
    string name;
    int x, y;
    mapping planet;

    if (sscanf(file_name(), PLANET_V_ROOM + "surface/%s/%d.%d", name, x, y) != 3) {
        return 0;
    }

    planet = D_PLANET->query_planet(name);
    if (arrayp(planet["overrides"])) {
        foreach (mapping override in filter(planet["overrides"], (: $1["x"] == $(x) && $1["y"] == $(y) :))) {
            if (override["type"] == "dome") {
                return "%^AFF%^";
            }
        }
    }

    return ::query_room_square_color();
}
string *query_room_map () {
    string *result = ({ });
    mapping planet;
    string name, path, line, symbol;
    int size, radius = 2;
    int x, y, xx, yy, wx, wy;
    object room;

    if (sscanf(file_name(), PLANET_V_ROOM + "surface/%s/%d.%d", name, x, y) != 3) {
        return 0;
    }

    path = PLANET_V_ROOM + "surface/" + name;
    planet = D_PLANET->query_planet(name);
    size = D_PLANET->query_planet_size(name);

    for (yy = -radius; yy <= radius; yy ++) {
        line = "";
        for (xx = -radius; xx <= radius; xx ++) {
            // if (sqrt(xx * xx + yy * yy) - 0.5 > radius) {
            //     line += "   ";
            //     continue;
            // }
            if (xx == 0 && yy == 0) {
                room = this_object();
                symbol = "%^CYAN%^BOLD%^X%^RESET%^";
            } else {
                wx = (x + xx) > 0 ? (x + xx) : size - 1;
                wy = (y + yy) > 0 ? (y + yy) : size - 1;
                room = load_object(path + "/" + wx + "." + wy + ".c");
                // @TODO check monster/resource?
                symbol = " ";
            }
            if (room) {
                line += room->query_room_square_color() + "[" + symbol + room->query_room_square_color() + "]%^RESET%^";
            } else {
                line += "   ";
            }
        }
        result += ({ line });
    }

    return result;
}