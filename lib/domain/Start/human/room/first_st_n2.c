#include <human.h>

inherit STD_ROOM;

void create () {
    ::create();
    set_short("a village street");
    set_long("A small street traveling north to south through brick dwellings covered in dark ivy. Several birch trees branch over the street and dwellings. A framework of hexagons glimmer overhead.");
    set_exits(([
        "south": HUMAN_ROOM + "first_st_n1.c",
        "north": HUMAN_ROOM + "chestnut_st_w6.c",
        "enter west": HUMAN_ROOM + "building_normal3.c",
    ]));
    set_looks(([
        "street": "A street sign is labeled N. First St.",
        ({ "framework", "hexagons"}): "Far above, the hexagons form a geodesic dome.",
    ]));
}