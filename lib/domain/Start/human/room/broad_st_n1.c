#include "human.h"

inherit STD_ROOM;

void create () {
    ::create();
    set_short("a village street");
    set_long("A small street traveling north and south through brick dwellings covered in dark ivy. Several birch trees branch over the street and dwellings. A public square can be seen to the south. A framework of hexagons glimmer overhead.");
    set_exits(([
        "north": HUMAN_ROOM + "broad_st_n2.c",
        "south": HUMAN_ROOM + "square.c",
    ]));
    set_looks(([
        "street": "A village street labeled N Broad St.",
        "square": "A square plaza with trees, benches, and more.",
    ]));
}