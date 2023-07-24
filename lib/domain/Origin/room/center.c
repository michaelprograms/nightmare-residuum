#include "origin.h"

inherit STD_ROOM;

void create () {
    ::create();
    set_properties(([
        "no attack": 1,
    ]));
    set_short("Origin Point");
    set_long("A central room with exits leading from the corners. The walls are a utility corridor white and lacking in any form of markers.");
    set_exits(([
        "northwest": ORIGIN_ROOM + "northwest.c",
        "northeast": ORIGIN_ROOM + "northeast.c",
        "southeast": ORIGIN_ROOM + "southeast.c",
        "southwest": ORIGIN_ROOM + "southwest.c",
    ]));
    set_reset(([
        ORIGIN_NPC + "greeter.c": 1,
    ]));
}