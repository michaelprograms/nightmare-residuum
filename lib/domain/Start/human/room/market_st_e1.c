#include "human.h"

inherit STD_ROOM;

void create () {
    ::create();
    set_short("a village street");
    set_long("A small street traveling north and south through brick dwellings covered in dark ivy. Several birch trees branch over the street and dwellings. A building with a glass window is on the south side of the street. A public square can be seen to the west. A framework of hexagons glimmer overhead.");
    set_exits(([
        "east": HUMAN_ROOM + "market_st_e2.c",
        "west": HUMAN_ROOM + "square.c",
        "enter south": HUMAN_ROOM + "store_clothing.c",
    ]));
    set_looks(([
        "street": "A village street labeled E Market St.",
        "square": "A square plaza with trees, benches, and more.",
    ]));
}