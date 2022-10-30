#include "human.h"

inherit STD_ROOM;

void create () {
    ::create();
    set_short("a village street");
    set_long("A small street traveling south through brick dwellings covered in dark ivy. Several birch trees branch over the street and dwellings. A framework of hexagons glimmer overhead. A concrete wall at the base of the hexagon dome blocks further passage north.");
    set_exits(([
        "south": HUMAN_ROOM + "fourth_st_n5.c",
    ]));
    set_looks(([
        "street": "A street sign is labeled N. Fourth St.",
    ]));
}