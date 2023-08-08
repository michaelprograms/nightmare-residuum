#include "origin.h"

inherit STD_ROOM;

void create () {
    ::create();
    set_properties(([
        "no attack": 1,
    ]));
    set_short("Origin Point");
    set_long("A central chamber with hallways branching from the corners of the room. The walls are a utility corridor white and lacking in any form of markers. A tank stands on the north side of the chamber.");
    set_exits(([
        "northwest": ORIGIN_ROOM + "northwest.c",
        "northeast": ORIGIN_ROOM + "northeast.c",
        "southeast": ORIGIN_ROOM + "southeast.c",
        "southwest": ORIGIN_ROOM + "southwest.c",
    ]));
    set_looks(([
        "chamber": "The center of the Origin Point is a large chamber. A hallway branches out diagonally from each corner of the room.",
        "hallway": "Four hallways of very utilitarian hallway design, simple but built to last.",
        "tank": "It appears to be a chamber for cloning a living organism.",
    ]));
    set_reset(([
        ORIGIN_NPC + "greeter.c": 1,
    ]));
}