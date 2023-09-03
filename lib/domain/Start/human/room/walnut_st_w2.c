#include "human.h"

inherit STD_ROOM;

void create () {
    ::create();
    set_short("a village street");
    set_long("A small street traveling west to east through brick dwellings covered in dark ivy. Several birch trees branch over the street and dwellings. An open grassy area is to the west. A framework of hexagons glimmer overhead.");
    set_exits(([
        "east": HUMAN_ROOM + "walnut_st_w1.c",
        "west": HUMAN_ROOM + "park_sw.c",
    ]));
    set_climbs(([
        "ivy": HUMAN_ROOM + "roof.c",
        "up": HUMAN_ROOM + "roof.c",
    ]));
    set_looks(([
        "street": "A village street labeled W Walnut St.",
        ({ "framework", "hexagons"}): "Far above, the hexagons form a geodesic dome.",
    ]));
}