#include <human.h>

inherit STD_ROOM;

void create () {
    ::create();
    set_short("a village street");
    set_long("A small street traveling west to east through brick dwellings covered in dark ivy. Several birch trees branch over the street and dwellings. A framework of hexagons glimmer overhead.");
    set_exits(([
        "west": HUMAN_ROOM + "spruce_st_e1.c",
        "east": HUMAN_ROOM + "fourth_st_n6.c",
        "enter north": HUMAN_ROOM + "building_brewery.c",
    ]));
    set_looks(([
        "street": "A street sign is labeled E. Spruce St.",
        ({ "framework", "hexagons"}): "Far above, the hexagons form a geodesic dome.",
    ]));
}