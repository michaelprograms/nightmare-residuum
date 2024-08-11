#include <human.h>

inherit STD_ROOM;

void create () {
    ::create();
    set_short("a village cross-street");
    set_long("A small street traveling west to east through brick dwellings covered in dark ivy, intersecting a road traveling north to south and a third street that begins heading southwest here. Several birch trees branch over the street and dwellings. A framework of hexagons glimmer overhead.");
    set_exits(([
        "southwest": HUMAN_ROOM + "avenue_n2.c",
        "north": HUMAN_ROOM + "fifth_st_n1.c",
        "east": HUMAN_ROOM + "market_st_e7.c",
        "south": HUMAN_ROOM + "fifth_st_s1.c",
        "west": HUMAN_ROOM + "market_st_e5.c",
    ]));
    set_looks(([
        "street": "A street sign is labeled Fifth Street, E. Market St, and N. Avenue.",
        ({ "framework", "hexagons"}): "Far above, the hexagons form a geodesic dome.",
    ]));
}