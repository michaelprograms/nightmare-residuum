#include <planet.h>

inherit STD_ROOM;

int is_virtual_room () { return 1; }

void create() {
    ::create();
    set_short("a terrain somewhere");
    set_long("The terrain of a planet.");
}

/* ----- biome ----- */

void set_biome (string biome) {
    set_property("biome", biome);

    switch (biome) {
    case "deeper water":
        set_room_square_color("\e[38;2;0;96m");           // #000060
        set_short("deeper water");
        set_long("Surrounded by deeper water.");
        break;
    case "deep water":
        set_room_square_color("\e[38;2;0;128m");           // #000080
        set_short("deep water");
        set_long("Surrounded by deep water.");
        break;
    case "shallow water":
        set_room_square_color("\e[38;2;25;25;150m");        // #191996
        set_short("water");
        set_long("Surrounded by water.");
        break;
    case "ice":
        set_room_square_color("\e[38;2;0;0;0m");            // #FFFFFF
        set_short("ice");
        set_long("Surrounded by ice.");
        break;
    case "tundra":
        set_room_square_color("\e[38;2;96;131;112m");       // #608370
        set_short("tundra");
        set_long("Surrounded by tundra.");
        break;
    case "grassland":
        set_room_square_color("\e[38;2;164;255;99m");       // #A4FF63
        set_short("grassland");
        set_long("Surrounded by grassland.");
        break;
    case "woodland":
        set_room_square_color("\e[38;2;139;175;90m");   // #8BAF5A
        set_short("woodland");
        set_long("Surrounded by woodland.");
        break;
    case "boreal forest":
        set_room_square_color("\e[38;2;95;115;62m");    // #5F733E
        set_short("boreal forest");
        set_long("Surrounded by boreal forest.");
        break;
    case "desert":
        set_room_square_color("\e[38;2;238;218;130m");  // #EEDA82
        set_short("desert");
        set_long("Surrounded by desert.");
        break;
    case "woodland":
        set_room_square_color("\e[38;2;139;175;90m");   // #8BAF5A
        set_short("woodland");
        set_long("Surrounded by woodland.");
        break;
    case "seasonal forest":
        set_room_square_color("\e[38;2;73;100;35m");    // #496423
        set_short("seasonal forest");
        set_long("Surrounded by seasonal forest.");
        break;
    case "temperate rainforest":
        set_room_square_color("\e[38;2;29;73;40m");     // #1D4928
        set_short("temperate rainforest");
        set_long("Surrounded by temperate rainforest.");
        break;
    case "savanna":
        set_room_square_color("\e[38;2;177;209;110m");  // #B1D16E
        set_short("savanna");
        set_long("Surrounded by savanna.");
        break;
    case "tropical rainforest":
        set_room_square_color("\e[38;2;66;123;25m");    // #427B19
        set_short("tropical rainforest");
        set_long("Surrounded by tropical rainforest.");
        break;
    default:
        set_room_square_color("%^RED%^");
        set_short("error");
        set_long("Error.");
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