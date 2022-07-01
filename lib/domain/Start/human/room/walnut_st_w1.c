#include "human.h"

inherit STD_ROOM;

void create () {
    ::create();
    set_short("a village street");
    set_long("A small street traveling west and east through brick dwellings covered in dark ivy. Several birch trees branch over the street and dwellings. A framework of hexagons glimmer overhead.");
    set_exits(([
        "east": HUMAN_ROOM + "broad_st_s3.c",
        "west": HUMAN_ROOM + "walnut_st_w2.c",
    ]));
    set_looks(([
        "street": "A village street labeled W Walnut St.",
    ]));
}