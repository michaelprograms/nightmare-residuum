#include "origin.h"

inherit STD_ROOM;

void create () {
    ::create();
    set_properties(([
        "no attack": 1,
    ]));
    set_short("Origin Point");
    set_long("A wide hallway joining two side rooms. The walls are utility corridor white and lacking in any form of markers. A window on the eastern wall shows a grey moon on a backdrop of stars.");
    set_exits(([
        "north": ORIGIN_ROOM + "northeast.c",
        "south": ORIGIN_ROOM + "southeast.c",
    ]));
    set_looks(([
        "hallway": "This hallway connects two rooms.",
        "walls": "Plain white walls, free of blemishes or signage.",
        "window": "A wide window set into the eastern wall provides a view of a grey moon.",
    ]));
    set_reset(([
        ORIGIN_ITEM + "transporter.c": 1,
    ]));
}