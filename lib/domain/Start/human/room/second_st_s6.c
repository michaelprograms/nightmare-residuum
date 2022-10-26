#include "human.h"

inherit STD_ROOM;

void create () {
    ::create();
    set_short("a village street");
    set_long("A small street traveling north through brick dwellings covered in dark ivy. Several birch trees branch over the street and dwellings. A framework of hexagons glimmer overhead. A concrete wall at the base of the hexagon dome blocks further passage south.");
    set_exits(([
        "north": HUMAN_ROOM + "second_st_s5.c",
        "east": HUMAN_ROOM + "sassafras_st_w2.c",
    ]));
    set_looks(([
        "street": "A street sign is labeled S. Second St.",
    ]));
}