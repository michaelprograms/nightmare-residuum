#include "human.h"

inherit STD_ROOM;

void create () {
    ::create();
    set_short("a village street");
    set_long("A small street traveling west to east through brick dwellings covered in dark ivy. Several birch trees branch over the street and dwellings. A large marble building is to the north. A framework of hexagons glimmer overhead.");
    set_exits(([
        "west": HUMAN_ROOM + "chestnut_st_w6.c",
        "east": HUMAN_ROOM + "chestnut_st_w4.c",
    ]));
    set_looks(([
        "street": "A village street labeled W Chestnut St.",
        "building": "A large marble building that takes spans the width of the street block. It looks to be a couple stories tall with an ornate crowning around the roof line. Some windows are spaced out along the side of the building. Columns span the entranceway.",
        ({ "framework", "hexagons"}): "Far above, the hexagons form a geodesic dome.",
    ]));
}