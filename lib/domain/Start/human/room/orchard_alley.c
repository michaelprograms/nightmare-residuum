#include "human.h"

inherit STD_ROOM;

void create () {
    ::create();
    set_short("a village alley");
    set_long("A small alley traveling west and east through brick dwellings covered in dark ivy. A village street can be seen to the west. An orchard spans in the distance to the east. A framework of hexagons glimmer overhead.");
    set_exits(([
        "west": HUMAN_ROOM + "broad_st_n3.c",
        "east": HUMAN_ROOM + "orchard_entrance.c",
    ]));
    set_looks(([
        "street": "A village street down the alley the west.",
        "orchard": "The orchard of trees looks to contain several different types of trees.",
    ]));
}