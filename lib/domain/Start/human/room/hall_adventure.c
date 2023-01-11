#include "human.h"

inherit STD_ROOM;

void create () {
    room::create();
    train::create();
    set_properties(([
        "no attack": 1,
    ]));
    set_short("a village training hall");
    set_long("The inside of a brick building that consists of a large open hall. The doorway on the west wall is held open by ropes attached to hooks. There is training equipment of all sorts placed throughout the room.");
    set_exits(([
        "out west": HUMAN_ROOM + "broad_st_n1.c",
    ]));
}