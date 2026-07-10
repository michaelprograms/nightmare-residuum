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

void update_contents () {
    int level = query_property("level");
    string npc;

    set_reset_data(([ ]));
    if (sizeof(filter(query_living_contents(), (: npcp :))) > 0) {
        return;
    }
    npc = element_of(({ "feral_cat", "wild_dog", "rabid_rat", "large_ant", "plasma_snail", }));
    set_reset_data(([
        PLANET_NPC + npc + ".c": ([
            "number": 1,
            "setup": (: $1->set_level($(level)) :),
        ]),
    ]));
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
