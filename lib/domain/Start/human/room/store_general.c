#include "human.h"

inherit STD_ROOM;

void create () {
    ::create();
    set_properties(([
        "no attack": 1,
    ]));
    set_short("a small store");
    set_long("The inside of a brick building that consists of a small room. There are two small glass windows on the south facing wall with a door to the side. There are several shelves of items. Behind a counter are several small shelves of items.");
    set_exits(([
        "out north": HUMAN_ROOM + "market_st_e1.c",
    ]));
    set_looks(([
        "shelves": "Shelves containing some items for sale.",
        "counter": "A wooden counter spanning the room and bisecting it nearly in the middle.",
        "windows": "The windows look south out to the street.",
    ]));
    set_reset(([
        HUMAN_NPC + "vendor_general.c": 1,
    ]));
}
