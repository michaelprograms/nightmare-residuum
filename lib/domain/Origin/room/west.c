#include "origin.h"
#include "/domain/Start/include/human.h"

inherit STD_ROOM;

void create () {
    ::create();
    set_properties(([
        "no attack": 1,
    ]));
    set_short("Origin Point");
    set_long("A wide hallway joining two side rooms. The walls are utility corridor white and lacking in any form of markers. A window on the western wall shows a blue and green planet on a backdrop of stars.");
    set_exits(([
        "north": ORIGIN_ROOM + "northwest.c",
        "south": ORIGIN_ROOM + "southwest.c",
    ]));
    set_exit("pad", HUMAN_ROOM + "square.c", 0, 0, "thin air");
    set_looks(([
        "hallway": "This hallway is of very utilitarian hallway design, simple but built to last. It connects two rooms together.",
        "walls": "Plain white walls, free of blemishes or signage.",
        "window": "A wide window set into the western wall provides a view of a blue and green planet. Clouds can be seen at various levels of the atmosphere.",
    ]));
    set_reset(([
        ORIGIN_ITEM + "transporter.c": 1,
    ]));
}