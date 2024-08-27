#include "origin.h"

inherit STD_ROOM;

void create () {
    ::create();
    set_properties(([
        "no attack": 1,
    ]));
    set_short("Origin Point");
    set_long("A side chamber with exits leading out from the southwest corner. The walls are utility corridor white and lacking in any form of markers.");
    set_exits(([
        "west": ORIGIN_ROOM + "north.c",
        "south": ORIGIN_ROOM + "east.c",
        "southwest": ORIGIN_ROOM + "center.c",
    ]));
    set_looks(([
        "chamber": "The chamber off of a larger chamber, a main hallway branches out diagonally towards the center while smaller hallways lead to neighboring chambers.",
        "hallway": "This hallway is of very utilitarian hallway design, simple but built to last. It connects two rooms together.",
    ]));
}