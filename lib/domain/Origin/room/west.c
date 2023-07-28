#include "origin.h"
#include "/domain/Start/include/human.h"

inherit STD_ROOM;

void create () {
    ::create();
    set_properties(([
        "no attack": 1,
    ]));
    set_short("Origin Point");
    set_long("A wide hallway with joining two side rooms. The walls are a utility corridor white and lacking in any form of markers.");
    set_exits(([
        "north": ORIGIN_ROOM + "northwest.c",
        "south": ORIGIN_ROOM + "southwest.c",
        "pad": HUMAN_ROOM + "square.c",
    ]));
    set_reset(([
        ORIGIN_ITEM + "transporter.c": 1,
    ]));
}