#include "origin.h"

inherit STD_ROOM;

void create () {
    ::create();
    set_properties(([
        "no attack": 1,
    ]));
    // set_short("");
    // set_long("");
    set_exits(([
        "east": ORIGIN_ROOM + "north.c",
        "south": ORIGIN_ROOM + "west.c",
        "southeast": ORIGIN_ROOM + "center.c",
    ]));
}