#include "human.h"

inherit STD_ROOM;

void create () {
    room::create();
    set_properties(([
        "no attack": 1,
        "indoors": 1,
    ]));
    set_short("a large hall");
    set_long("The inside of a brick building that consists of a large open hall. The doorway on the west wall is held open by ropes attached to hooks. Adventurers lounge around the hall between journeys.");
    set_exits(([
        "out west": HUMAN_ROOM + "broad_st_n1.c",
    ]));
}