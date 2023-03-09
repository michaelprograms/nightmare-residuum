#include "human.h"

inherit STD_ROOM;

void create () {
    ::create();
    set_short("a village street");
    set_long("A small street traveling west to east through brick dwellings covered in dark ivy. Several birch trees branch over the street and dwellings. A building with a large glass window is on the south side of the street. A public square can be seen to the east surrounding an obelisk. A framework of hexagons glimmer overhead.");
    set_exits(([
        "east": HUMAN_ROOM + "square.c",
        "west": HUMAN_ROOM + "market_st_w2.c",
        "enter south": HUMAN_ROOM "bank.c",
    ]));
    set_looks(([
        "street": "A street sign is labeled W. Market St.",
        "square": "A square plaza with trees, benches, and more.",
    ]));
}