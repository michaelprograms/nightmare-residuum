#include <planet.h>

inherit STD_ROOM;

int is_virtual_room () { return 1; }

/* ----- biome ----- */

void update_descriptions () {
    string biome = query_property("biome");
    string color = D_PLANET->query_biome_color_ansi(biome);

    set_room_bracket_color(color);
    set_short(biome);

    switch (biome) {
        case "frozen water":
            set_long("Surrounded by frozen water.");
            set_property("water", 1);
            set_property("go", 4);
        case "icy water":
            set_long("Surrounded by icy water.");
            set_property("water", 1);
            set_property("go", 4);
            break;
        case "deeper water":
            set_long("Surrounded by deeper water.");
            set_property("water", 3);
            set_property("go", 5);
            break;
        case "deep water":
            set_long("Surrounded by deep water.");
            set_property("water", 2);
            set_property("go", 4);
            break;
        case "shallow water":
            set_long("Surrounded by water.");
            set_property("water", 1);
            set_property("go", 3);
            break;
        case "ice":
            set_long("Surrounded by ice.");
            set_property("go", 3);
            break;
        case "tundra":
            set_long("Surrounded by tundra.");
            set_property("go", 2);
            break;
        case "grassland":
            set_long("Surrounded by grassland.");
            set_property("go", 1);
            break;
        case "woodland":
            set_long("Surrounded by woodland.");
            set_property("go", 1);
            break;
        case "boreal forest":
            set_long("Surrounded by boreal forest.");
            set_property("go", 2);
            break;
        case "desert":
            set_long("Surrounded by desert.");
            set_property("go", 2);
            break;
        case "seasonal forest":
            set_long("Surrounded by seasonal forest.");
            set_property("go", 1);
            break;
        case "temperate rainforest":
            set_long("Surrounded by temperate rainforest.");
            set_property("go", 4);
            break;
        case "savanna":
            set_long("Surrounded by savanna.");
            set_property("go", 2);
            break;
        case "tropical rainforest":
            set_long("Surrounded by tropical rainforest.");
            set_property("go", 4);
            break;
        default:
            set_long("Error: unknown biome.");
            break;
    }
}

void add_terrain_override (string text) {
    set_long(query_long() + " " + text);
}

void update_resource () {
    int nLevel = query_property("level");
    int nResource = query_property("resource");
    object obResource = present("resource_node");

    // clear existing reset
    set_reset_data(([ ]));

    if (query_property("no resource")) {
        return;
    }

    if (nResource == 1) { // ore
        if (!obResource) {
            obResource = new("/std/resource/harvestable.c");
            obResource->set_type("ore");
            obResource->set_level(nLevel);
            obResource->handle_move(this_object());
        } else {
            obResource->reset();
        }
    } else if (nResource == 2) { // wood
        if (!obResource) {
            obResource = new("/std/resource/harvestable.c");
            obResource->set_type("wood");
            obResource->set_level(nLevel);
            obResource->handle_move(this_object());
        } else {
            obResource->reset();
        }
    } else if (nResource == 3 || nResource == 4) { // NPC
        string npc;

        if (sizeof(filter(query_living_contents(), (: $1->is_npc() :))) > 0) {
            return;
        }

        switch (random(2)) {
        case 0:
            npc = element_of(({
                "curious_raccoon",
                "feral_cat",
                "large_ant",
                "lurking_vulture",
                "rabid_rat",
                "watchful_crow",
                "wild_dog",
            }));
            break;
        case 1:
            npc = "plasma_snail.c";
            break;
        }
        set_reset_data(([
            PLANET_NPC + npc + ".c": ([
                "number": 1,
                "setup": (: $1->set_level($(nLevel)) :),
            ]),
        ]));
        handle_reset();
    } else {
        if (obResource = present("resource_node")) {
            obResource->handle_remove();
        }
    }
}

/* ----- exit override ----- */

mixed handle_go (object ob, string verb, string dir) {
    if (ob->query_cooldown("go")) {
        message("action", "You cannot go anywhere yet.", ob);
    } else {
        ob->set_cooldown("go", 0.2 * query_property("go"), "timed");
        ::handle_go(ob, verb, dir);
    }
}

/* ----- map override ----- */

string query_room_bracket_color () {
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

    return ::query_room_bracket_color();
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
                symbol = 0;
            }
            if (room) {
                line += room->query_room_map_bracket(symbol);
            } else {
                line += "   ";
            }
        }
        result += ({ line });
    }

    return result;
}

/* ----- applies ----- */

void create () {
    ::create();
    set_short("a planet somewhere");
    set_long("The terrain of a planet.");
}

void reset () {
    if (
        !query_property("no resource") &&
        query_property("name") &&
        query_property("x") &&
        query_property("y")
    ) {
        PLANET_SURFACE->setup_room(this_object());
        update_descriptions();
        update_resource();
    }

    room::reset();
}

/* ----- container override ----- */

int can_receive (object ob) {
    if (query_property("no receive")) {
        return 0;
    } else {
        return ::can_receive(ob);
    }
}