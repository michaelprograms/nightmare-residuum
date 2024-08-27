#include "origin.h"

inherit STD_ROOM;

void create () {
    ::create();
    set_properties(([
        "no attack": 1,
    ]));
    set_short("Origin Point");
    set_long("A wide hallway joining two side rooms. The walls are utility corridor white and lacking in any form of markers. A window on the northern wall shows a backdrop of stars.");
    set_exits(([
        "west": ORIGIN_ROOM + "northwest.c",
        "east": ORIGIN_ROOM + "northeast.c",
    ]));
    set_looks(([
        "hallway": "This hallway is of very utilitarian hallway design, simple but built to last. It connects two rooms together.",
        "walls": "Plain white walls, free of blemishes or signage.",
        "window": "A wide window set into the northern wall provides a view of the stars.",
    ]));
    set_reset(([
        ORIGIN_ITEM + "transporter.c": 1,
    ]));
}