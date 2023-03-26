#include "human.h"

inherit STD_ROOM;

void create () {
    ::create();
    set_properties(([
        "no attack": 1,
        "indoors": 1,
    ]));
    set_short("a weapon store");
    set_long("The inside of a brick building that consists of an open room. There is a glass window to the south facing the street with a door to the side and a display of weapons in the window. Behind the counter are several more racks of weapons.");
    set_exits(([
        "out south": HUMAN_ROOM + "market_st_e2.c",
    ]));
    set_looks(([
        "racks": "Racks containing the weapons available for sale here.",
        "counter": "A wooden counter spanning the room and bisecting it nearly in the middle.",
        "window": "Weapons are arranged in the window to display the merchandise.",
    ]));
    set_reset(([
        HUMAN_NPC + "vendor_weapon.c": 1,
    ]));
}
