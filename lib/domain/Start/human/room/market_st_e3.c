#include "human.h"

inherit STD_ROOM;

void create () {
    ::create();
    set_short("a village crossroads");
    set_long("A small street traveling west to east through brick dwellings covered in dark ivy. Several birch trees branch over the street and dwellings. An obelisk can be seen to the west. A framework of hexagons glimmer overhead.");
    set_exits(([
        "east": HUMAN_ROOM + "market_st_e4.c",
        "west": HUMAN_ROOM + "market_st_e2.c",
        "north": HUMAN_ROOM + "fourth_st_n1.c",
        "south": HUMAN_ROOM + "fourth_st_s1.c",
    ]));
    set_looks(([
        "street": "A street sign is labeled E. Market St.",
        ({ "framework", "hexagons"}): "Far above, the hexagons form a geodesic dome.",
    ]));
}