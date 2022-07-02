#include "human.h"

inherit STD_ROOM;

void create () {
    ::create();
    set_short("a village street");
    set_long("A small street traveling west and east through brick dwellings covered in dark ivy. Several birch trees branch over the street and dwellings. A framework of hexagons glimmer overhead.");
    set_exits(([
        "west": HUMAN_ROOM + "chestnut_st_e1.c",
        "east": HUMAN_ROOM + "park_northeast.c",
    ]));
    set_looks(([
        "street": "A village street labeled E Chestnut St.",
    ]));
}