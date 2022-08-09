#include "human.h"

inherit STD_ROOM;

void create () {
    ::create();
    set_short("a village street");
    set_long("A small street traveling north to south through brick dwellings covered in dark ivy. Several birch trees branch over the street and dwellings. An open grassy area is to the south. A framework of hexagons glimmer overhead.");
    set_exits(([
        "north": HUMAN_ROOM + "pine_st_n5.c",
        "south": HUMAN_ROOM + "park_northwest.c",
    ]));
    set_looks(([
        "street": "A village street labeled N Pine St.",
    ]));
}