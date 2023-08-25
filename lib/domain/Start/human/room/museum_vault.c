#include "human.h"

inherit STD_ROOM;

void create () {
    ::create();
    set_properties(([ "indoors": 1, "no attack": 1, ]));
    set_short("a chamber inside a building");
    set_long("A chamber of a room with high ceiling and polished floors. The walls are made of stone blocks and covered with tapestries. Archway lead out of the chamber into a hallways to the east and south.");
    set_exits(([
        "east": HUMAN_ROOM + "museum_1_n1.c",
        "south": HUMAN_ROOM + "museum_1_w1.c",
    ]));
    set_looks(([
        "floor": "The floor is a polished white stone in large blocks.",
        "chamber": "This chamber is an open room but not quite as large as the rest of the building.",
        ({ "archway", "hallway", }): "Hallway leading north-south and west-east are visible on the other sides of the archways in the stone walls to the east and south.",
    ]));
    set_reset(([

    ]));
}