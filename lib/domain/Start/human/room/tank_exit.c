#include "human.h"

inherit STD_ROOM;

void create () {
    ::create();
    set_short("a hallway");
    set_long("The end of a hallway in front of a door to a small enclosed room.");
    set_exits(([
        "south": HUMAN_ROOM + "tank_hallway2.c",
        "enter north": HUMAN_ROOM + "tank_elevator.c",
    ]));
}