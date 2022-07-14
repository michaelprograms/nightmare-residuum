#include "human.h"

inherit STD_ROOM;

void create () {
    ::create();
    set_short("a village street");
    set_long("A small street traveling north and south through brick dwellings covered in dark ivy. Several birch trees branch over the street and dwellings. An open grassy area is to the north. An open grassy area is to the south. A framework of hexagons glimmer overhead.");
    set_exits(([
        "south": HUMAN_ROOM + "pine_st_s5.c",
        "north": HUMAN_ROOM + "park_southwest.c",
    ]));
    set_looks(([
        "street": "A village street labeled S Pine St.",
    ]));
}