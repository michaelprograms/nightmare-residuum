#include "human.h"

inherit STD_ROOM;

void create () {
    ::create();
    set_short("a village crossroads");
    set_long("A small street traveling north to south through brick dwellings covered in dark ivy. Several birch trees branch over the street and dwellings. An obelisk can be seen to the south. A framework of hexagons glimmer overhead.");
    set_exits(([
        "west": HUMAN_ROOM + "chestnut_st_w1.c",
        "east": HUMAN_ROOM + "chestnut_st_e1.c",
        "north": HUMAN_ROOM + "broad_st_n4.c",
        "south": HUMAN_ROOM + "broad_st_n2.c",
    ]));
    set_looks(([
        "street": "A street sign is labeled N. Broad St.",
    ]));
}