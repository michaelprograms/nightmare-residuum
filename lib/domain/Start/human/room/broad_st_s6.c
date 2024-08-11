#include <human.h>

inherit STD_ROOM;

void create () {
    ::create();
    set_short("a village cross-street");
    set_long("A small street traveling north to south through brick dwellings covered in dark ivy, intersecting a street traveling west to east and a third street that begins heading northeast here. Several birch trees branch over the street and dwellings. A framework of hexagons glimmer overhead.");
    set_exits(([
        "northeast": HUMAN_ROOM + "avenue_s2.c",
        "north": HUMAN_ROOM + "broad_st_s5.c",
        "west": HUMAN_ROOM + "pine_st_w1.c",
        "east": HUMAN_ROOM + "pine_st_e1.c",
        "south": HUMAN_ROOM + "broad_st_s7.c",
    ]));
    set_looks(([
        "street": "A street sign is labeled Pine St, S. Broad St, and S. Avenue.",
        ({ "framework", "hexagons"}): "Far above, the hexagons form a geodesic dome.",
    ]));
}