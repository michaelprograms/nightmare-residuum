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
        "north": ORIGIN_ROOM + "northeast.c",
        "south": ORIGIN_ROOM + "southeast.c",
    ]));
}