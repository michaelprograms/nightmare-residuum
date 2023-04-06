#include "human.h"

inherit STD_ROOM;

void create () {
    ::create();
    set_short("a village street");
    set_long("A small street traveling east through brick dwellings covered in dark ivy. Several birch trees branch over the street and dwellings. A framework of hexagons glimmer overhead. A concrete wall at the base of the hexagon dome blocks further passage west.");
    set_exits(([
        "east": HUMAN_ROOM + "chestnut_st_w5.c",
        "south": HUMAN_ROOM + "first_st_n2.c",
    ]));
    set_looks(([
        "street": "A village street labeled W Chestnut St.",
        ({ "framework", "hexagons"}): "Far above, the hexagons form a geodesic dome.",
    ]));
}