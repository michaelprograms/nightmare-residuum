#include "human.h"

inherit STD_ROOM;

void create () {
    ::create();
    set_short("a village street");
    set_long("A small street traveling north to south through brick dwellings covered in dark ivy. Several birch trees branch over the street and dwellings. A building on the west side of the street has benches set up and large open doors. A public square can be seen to the north. A framework of hexagons glimmer overhead.");
    set_exits(([
        "north": HUMAN_ROOM + "square.c",
        "south": HUMAN_ROOM + "broad_st_s2.c",
        "enter west": HUMAN_ROOM + "store_pub.c",
    ]));
    set_looks(([
        "street": "A village street labeled S Broad St.",
        "square": "A square plaza with trees, benches, and more.",
    ]));
}
