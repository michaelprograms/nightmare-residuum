#include "origin.h"

inherit STD_ROOM;

void create () {
    ::create();
    set_properties(([
        "no attack": 1,
    ]));
    set_short("Origin Point");
    set_long("A wide hallway joining two side rooms. The walls are utility corridor white and lacking in any form of markers.");
    set_exits(([
        "north": ORIGIN_ROOM + "northeast.c",
        "south": ORIGIN_ROOM + "southeast.c",
    ]));
    set_reset(([
        ORIGIN_ITEM + "transporter.c": 1,
    ]));
}