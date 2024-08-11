#include <human.h>

inherit STD_ROOM;

void create () {
    ::create();
    set_short("a village cross-street");
    set_long("A small street traveling north to south through brick dwellings covered in dark ivy, intersecting a road traveling west to east. Several birch trees branch over the street and dwellings. A framework of hexagons glimmer overhead.");
    set_exits(([
        "north": HUMAN_ROOM + "broad_st_n7.c",
        "south": HUMAN_ROOM + "broad_st_n5.c",
        "west": HUMAN_ROOM + "spruce_st_w1.c",
        "east": HUMAN_ROOM + "spruce_st_e1.c",
    ]));
    set_looks(([
        "street": "A street sign is labeled Spruce St and N. Broad St.",
        ({ "framework", "hexagons"}): "Far above, the hexagons form a geodesic dome.",
    ]));
}