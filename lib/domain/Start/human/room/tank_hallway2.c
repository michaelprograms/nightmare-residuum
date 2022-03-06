#include "human.h"

inherit STD_ROOM;

void create () {
    ::create();
    set_short("a hallway junction");
    set_long("A hallway junction in front of a tank. A number 2 is etched onto the wall. One hallway leads between the three tanks while the other splits away.");
    set_exits(([
        "west": HUMAN_ROOM + "tank_hallway1.c",
        "east": HUMAN_ROOM + "tank_hallway3.c",
        "north": HUMAN_ROOM + "tank_exit.c",
    ]));
}