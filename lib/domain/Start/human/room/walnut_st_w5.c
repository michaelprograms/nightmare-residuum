#include <human.h>

inherit STD_ROOM;

void create () {
    ::create();
    set_short("a village street");
    set_long("A small street traveling west to east through brick dwellings covered in dark ivy. Several birch trees branch over the street and dwellings. A framework of hexagons glimmer overhead.");
    set_exits(([
        "west": HUMAN_ROOM + "walnut_st_w6.c",
        "east": HUMAN_ROOM + "walnut_st_w4.c",
        "enter south": HUMAN_ROOM + "building_nice2a.c",
    ]));
    set_looks(([
        "street": "A street sign is labeled W. Walnut St.",
        ({ "framework", "hexagons"}): "Far above, the hexagons form a geodesic dome.",
    ]));
}