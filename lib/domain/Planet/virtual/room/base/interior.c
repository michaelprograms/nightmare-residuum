#include <planet.h>

inherit STD_ROOM;

int is_virtual_room () { return 1; }

/* ----- description ----- */

void update_descriptions () {
    string type = query_property("type");

    switch (type) {
        case "crashed_ship":
            set_short("the interior of a crashed ship");
            set_long("Twisted bulkheads and shattered consoles line the interior of a crashed vessel. Scorched panels hang loose from the walls, and the air is thick with the smell of burnt insulation.");
            break;
        case "ruin":
            set_short("crumbling ruins");
            set_long("Weathered stone and collapsed archways form the remains of a long-abandoned structure. Dust drifts through shafts of pale light falling from gaps in the roof overhead.");
            break;
        default:
            set_long("Error: unknown structure type.");
            break;
    }
}

/* ----- contents ----- */

private void setup_salvage (object node, int level) {
    node->set_type("salvage");
    node->set_level(level);
}

private void setup_npc (object npc, int level) {
    object hide = new("/std/resource/resource.c");

    npc->set_level(level);
    hide->set_type("hide");
    hide->set_level(level);
    hide->handle_move(npc);
}

void update_contents () {
    int level = query_property("level");
    string npc;
    mapping data;

    data = ([
        "/std/resource/harvestable.c": ([
            "number": 1,
            "setup": (: setup_salvage($1, $(level)) :),
        ]),
    ]);
    if (sizeof(filter(query_living_contents(), (: npcp :))) < 1) {
        npc = element_of(({ "feral_cat", "wild_dog", "rabid_rat", "large_ant", "plasma_snail", }));
        data[PLANET_NPC + npc + ".c"] = ([
            "number": 1,
            "setup": (: setup_npc($1, $(level)) :),
        ]);
    }
    set_reset_data(data);
    handle_reset();
}

/* ----- applies ----- */

void create () {
    ::create();
    set_properties(([ "indoors": 1, ]));
    set_short("the interior of a structure");
    set_long("The interior of a structure.");
}

void reset () {
    if (query_property("type")) {
        update_contents();
    }
    room::reset();
}
