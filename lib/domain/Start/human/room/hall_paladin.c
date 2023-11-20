#include "human.h"

inherit STD_ROOM;

void create () {
    ::create();
    set_properties(([
        "no attack": 1,
        "indoors": 1,
    ]));
    set_short("a paladin's hall");
    set_long("The basement floor of a brick building that consists of the personal training area of the paladins. It is a sparsely populated area where the paladins train in an ascetic lifestyle. Wooden floorboards overhead form a crude barrier between the first floor. A staircase leads back up to the first floor.");
    set_smell("default", "The air is damp and cool.");
    set_listen("default", "Floorboards quietly creak from footsteps above.");
    set_exits(([
        "up": HUMAN_ROOM + "intro_paladin.c",
    ]));
    set_looks(([
        "mats": "Mats used to help a paladin meditate.",
        "stairs": "The staircase lead back up to the first floor of the building.",
    ]));
}
