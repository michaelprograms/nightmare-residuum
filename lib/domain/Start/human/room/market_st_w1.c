#include "human.h"

inherit STD_ROOM;

void create () {
    ::create();
    set_short("a village street");
    set_long("A small street traveling west and east through brick dwellings covered in dark ivy. Several birch trees branch over the street and dwellings. A public square can be seen to the east. A framework of hexagons glimmer overhead.");
    set_exits(([
        "east": HUMAN_ROOM + "square.c",
        "west": HUMAN_ROOM + "market_st_w2.c",
    ]));
    set_looks(([
        "street": "A village street labeled W Market St.",
        "square": "A square plaza with trees, benches, and more.",
    ]));
}