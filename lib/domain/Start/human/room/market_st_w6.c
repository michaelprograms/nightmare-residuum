#include "human.h"

inherit STD_ROOM;

void create () {
    ::create();
    set_short("a village street");
    set_long("A small street traveling west to east through brick dwellings covered in dark ivy. Several birch trees branch over the street and dwellings. A framework of hexagons glimmer overhead.");
    set_exits(([
        "west": HUMAN_ROOM + "market_st_w7.c",
        "north": HUMAN_ROOM + "first_st_n1.c",
        "east": HUMAN_ROOM + "market_st_w5.c",
        "south": HUMAN_ROOM + "first_st_s1.c",
    ]));
    set_looks(([
        "street": "A street sign is labeled W. Market St.",
        ({ "framework", "hexagons"}): "Far above, the hexagons form a geodesic dome.",
    ]));
}