#include <human.h>

inherit STD_ROOM;

void create () {
    ::create();
    set_short("a village street");
    set_long("A small street traveling west to east through brick dwellings covered in dark ivy. Several birch trees branch over the street and dwellings. A framework of hexagons glimmer overhead.");
    set_exits(([
        "enter west": HUMAN_ROOM + "market_st_w8.c",
        "east": HUMAN_ROOM + "market_st_w6.c",
    ]));
    set_looks(([
        "street": "A street sign is labeled W. Market St.",
        ({ "framework", "hexagons"}): "Far above, the hexagons form a geodesic dome.",
    ]));
    set_reset(([
        HUMAN_NPC + "plasma_snail.c": 1,
    ]));
}