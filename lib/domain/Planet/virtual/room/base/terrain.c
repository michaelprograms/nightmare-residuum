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
            set_long("The surroundings are a vast, icy expanse of frozen water. The surface is slick and treacherous, reflecting the cold light of the sky.");
            set_property("water", 1);
            set_property("go", 4);
            break;
        case "icy water":
            set_long("The area is filled with icy water, with chunks of ice floating around. The water is frigid, and the cold bites at the skin.");
            set_property("water", 1);
            set_property("go", 4);
            break;
        case "deeper water":
            set_long("The water here is deep and dark, hinting at unknown depths below. Strong currents make it harder to stay afloat.");
            set_property("water", 3);
            set_property("go", 5);
            break;
        case "deep water":
            set_long("The surroundings consist of deep water, with a sense of vastness beneath. The depth makes it difficult to see what lies below the surface.");
            set_property("water", 2);
            set_property("go", 4);
            break;
        case "shallow water":
            set_long("Shallow water covers the area, with the ground easily visible beneath. The water is calm and only reaches up to knee height.");
            set_property("water", 1);
            set_property("go", 3);
            break;
        case "ice":
            set_long("The landscape is covered in ice, with frozen ground stretching out in all directions. The air is crisp and cold, with a biting chill.");
            set_property("go", 3);
            break;
        case "tundra":
            set_long("A tundra surrounds the area, a flat and treeless plain with sparse vegetation. The ground is covered in a mix of moss and small shrubs, and the air is cold and dry.");
            set_property("go", 2);
            break;
        case "grassland":
            set_long("Grassland stretches out in all directions, with tall grasses swaying in the wind. The open expanse is dotted with wildflowers and small animals.");
            set_property("go", 1);
            break;
        case "woodland":
            set_long("The area is a woodland, surrounded by trees and underbrush. The forest floor is covered in leaves, and the canopy above provides dappled shade.");
            set_property("go", 1);
            break;
        case "boreal forest":
            set_long("A boreal forest surrounds the area, characterized by dense coniferous trees. The air is cool, and the forest floor is covered in a thick layer of needles and moss.");
            set_property("go", 2);
            break;
        case "desert":
            set_long("The landscape is a desert, with vast stretches of sand and sparse vegetation. The sun is scorching, and the area is dotted with cacti and hardy shrubs.");
            set_property("go", 2);
            break;
        case "seasonal forest":
            set_long("The surroundings are a seasonal forest, where the foliage changes with the seasons. The forest is rich with diverse plant life and vibrant colors during different times of the year.");
            set_property("go", 1);
            break;
        case "temperate rainforest":
            set_long("The area is a temperate rainforest, with lush vegetation and a canopy of towering trees. The air is humid and the forest floor is covered in ferns and moss.");
            set_property("go", 4);
            break;
        case "savanna":
            set_long("The savanna stretches out, characterized by a mix of grassland and scattered trees. The environment is warm, with an open landscape that supports a variety of wildlife.");
            set_property("go", 2);
            break;
        case "tropical rainforest":
            set_long("A tropical rainforest surrounds the area, with dense vegetation and a canopy that blocks out much of the sunlight. The air is warm and humid, and the forest teems with exotic wildlife.");
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
                symbol = "%^I_CYAN%^BOLD%^X%^RESET%^";
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