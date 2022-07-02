#include "human.h"

inherit STD_ROOM;

void create () {
    ::create();
    set_short("a village street");
    set_long("A small street traveling north and south through brick dwellings covered in dark ivy. Several birch trees branch over the street and dwellings. A framework of hexagons glimmer overhead.");
    set_exits(([
        "north": HUMAN_ROOM + "broad_st_s2.c",
        "south": HUMAN_ROOM + "broad_st_s4.c",
        "west": HUMAN_ROOM + "walnut_st_w1.c",
        "east": HUMAN_ROOM + "walnut_st_e1.c",
    ]));
    set_looks(([
        "street": "A village street labeled S Broad St.",
    ]));
}