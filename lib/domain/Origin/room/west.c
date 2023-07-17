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
        "north": ORIGIN_ROOM + "northwest.c",
        "south": ORIGIN_ROOM + "southwest.c",
    ]));
}