#include "human.h"

inherit STD_ROOM;

void create () {
    ::create();
    set_short("a village bank");
    set_long("The inside of a brick building that consists of a large open room. There is a large glass window to the north facing the street with a door to the side. There is a vault towards the south of the building.");
    set_exits(([
        "out north": HUMAN_ROOM "market_st_w1.c",
    ]));
    set_looks(([
        "vault": "A closed and secured vault door with no way to see through.",
    ]));
}