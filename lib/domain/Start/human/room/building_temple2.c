#include <human.h>

inherit STD_ROOM;

void create () {
    ::create();
    set_properties(([
        "indoors": 1,
    ]));
    set_short("a temple hall");
    set_long("The basement floor of a brick building used as a temple. It is a sparsely populated area where the holy train in an ascetic lifestyle. Wooden floorboards overhead form a crude barrier between the first floor. A staircase leads back up to the first floor.");
    set_smell("default", "The air is damp and cool.");
    set_listen("default", "Floorboards quietly creak from footsteps above.");
    set_exits(([
        "up": HUMAN_ROOM + "building_temple1.c",
    ]));
    set_looks(([
        "mats": "Mats used to help the holy meditate.",
        "stairs": "The staircase lead back up to the first floor of the building.",
    ]));
    set_reset(([
        HUMAN_NPC + "adult_worship.c": 2,
    ]));
}
