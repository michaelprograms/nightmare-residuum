#include "human.h"

inherit STD_ROOM;

void create () {
    ::create();
    set_short("a historical building");
    set_long("Inside a building made out of large stone blocks. The walls are far apart and the room's ceiling is high above.");
    set_exits(([
        "out southeast": HUMAN_ROOM + "parkway_4.c",
        "east": HUMAN_ROOM + "museum_e1.c",
        "south": HUMAN_ROOM + "museum_w1.c",
    ]));
    set_looks(([

    ]));
    set_items(([

    ]));
}