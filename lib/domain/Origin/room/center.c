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
        "northwest": ORIGIN_ROOM + "northwest.c",
        "northeast": ORIGIN_ROOM + "northeast.c",
        "southeast": ORIGIN_ROOM + "southeast.c",
        "southwest": ORIGIN_ROOM + "southwest.c",
    ]));
    // set_reset(([

    // ]));
}