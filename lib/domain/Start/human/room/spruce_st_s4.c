#include "human.h"

inherit STD_ROOM;

void create () {
    ::create();
    set_short("a village street");
    set_long("A small street traveling north and south through brick dwellings covered in dark ivy. Several birch trees branch over the street and dwellings. An open grassy area is to the north. A framework of hexagons glimmer overhead.");
    set_exits(([
        "north": HUMAN_ROOM + "park_southeast.c",
        "south": HUMAN_ROOM + "spruce_st_s5.c",
    ]));
    set_looks(([
        "street": "A village street labeled S Spruce St.",
    ]));
}