#include <human.h>

inherit STD_ROOM;

void create () {
    ::create();
    set_short("a village street");
    set_long("A small street traveling north to south through brick dwellings covered in dark ivy. Several birch trees branch over the street and dwellings. A framework of hexagons glimmer overhead.");
    set_exits(([
        "north": HUMAN_ROOM + "market_st_w6.c",
        "south": HUMAN_ROOM + "first_st_s2.c",
    ]));
    set_looks(([
        "street": "A street sign is labeled S. First St.",
        ({ "framework", "hexagons"}): "Far above, the hexagons form a geodesic dome.",
    ]));
    set_reset(([
        HUMAN_NPC + "teen_preppy.c": 2,
    ]));
}