#include "human.h"

inherit STD_ROOM;

void create () {
    ::create();
    set_short("on a building's roof");
    set_long("Atop a building with a flat roof. A street passes by to the south and north. Ivy grows over the lip of the roof. Atop a building on its roof. A framework of hexagons glimmer overhead.");
    set_climbs(([
        "ivy": HUMAN_ROOM + "walnut_st_w2.c",
    ]));
    set_looks(([
        "ivy": "The ivy looks sturdy enough to climb.",
    ]));
}