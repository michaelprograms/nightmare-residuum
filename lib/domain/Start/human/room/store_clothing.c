#include "human.h"

inherit STD_ROOM;

void create () {
    ::create();
    set_properties(([
        "no attack": 1,
    ]));
    set_short("a village bank");
    set_long("The inside of a brick building that consists of an open room. There is a large glass window to the north facing the street with a door to the side and several mannequins wearing examples of the merchandise. Behind the counter are several racks of clothes.");
    set_exits(([
        "out north": HUMAN_ROOM "market_st_e1.c",
    ]));
    set_looks(([
        "mannequin": "Wooden dolls as large as a human, showing clothing examples that the store sells.",
        "racks": "Racks containing the clothing the merchant is selling.",
        "counter": "A wooden counter spanning the room and bisecting it nearly in the middle.",
    ]));
    set_reset(([
        HUMAN_NPC + "vendor_clothing.c": 1,
    ]));
}
