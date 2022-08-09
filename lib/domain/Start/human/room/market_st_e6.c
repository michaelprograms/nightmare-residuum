#include "human.h"

inherit STD_ROOM;

void create () {
    ::create();
    set_short("a village street");
    set_long("A small street traveling west to east through brick dwellings covered in dark ivy. Several birch trees branch over the street and dwellings. A framework of hexagons glimmer overhead.");
    set_exits(([
        "east": HUMAN_ROOM + "market_st_e7.c",
        "west": HUMAN_ROOM + "market_st_e5.c",
    ]));
    set_looks(([
        "street": "A village street labeled E Market St.",
    ]));
}