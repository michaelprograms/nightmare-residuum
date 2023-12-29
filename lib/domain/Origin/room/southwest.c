#include "origin.h"

inherit STD_ROOM;

void create () {
    ::create();
    set_properties(([
        "no attack": 1,
    ]));
    set_short("Origin Point");
    set_long("A side room with exits leading out from one corner. The walls are utility corridor white and lacking in any form of markers.");
    set_exits(([
        "east": ORIGIN_ROOM + "south.c",
        "north": ORIGIN_ROOM + "west.c",
        "northeast": ORIGIN_ROOM + "center.c",
    ]));
}