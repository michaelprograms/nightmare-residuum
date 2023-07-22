#include "origin.h"

inherit STD_ROOM;

void create () {
    ::create();
    set_properties(([
        "no attack": 1,
    ]));
    // set_short("");
    set_long("A side room with exits leading out from one corner. The walls are a utility corridor white and lacking in any form of markers.");
    set_exits(([
        "east": ORIGIN_ROOM + "north.c",
        "south": ORIGIN_ROOM + "west.c",
        "southeast": ORIGIN_ROOM + "center.c",
    ]));
}