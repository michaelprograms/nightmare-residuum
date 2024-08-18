#include <human.h>

inherit STD_ROOM;

void create () {
    ::create();
    set_short("a village street");
    set_long("A small street traveling north to south through brick dwellings covered in dark ivy. Several birch trees branch over the street and dwellings. An open grassy area is to the north. A cafe can be seen on the east side of the street. A framework of hexagons glimmer overhead.");
    set_exits(([
        "south": HUMAN_ROOM + "second_st_n1.c",
        "north": HUMAN_ROOM + "park_nw.c",
        "enter east": HUMAN_ROOM + "store_cafe.c",
    ]));
    set_looks(([
        "street": "A street sign is labeled N. Second St.",
        ({ "framework", "hexagons"}): "Far above, the hexagons form a geodesic dome.",
        "cafe": "A brick dwelling serving as a cafe, as seen through the windows.",
    ]));
}