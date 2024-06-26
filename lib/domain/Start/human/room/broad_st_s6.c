#include <human.h>

inherit STD_ROOM;

void create () {
    ::create();
    set_short("a village street");
    set_long("A small street traveling north to south through brick dwellings covered in dark ivy. Several birch trees branch over the street and dwellings. A framework of hexagons glimmer overhead.");
    set_exits(([
        "northeast": "broad_st_s5.c",
        "north": HUMAN_ROOM + "broad_st_s5.c",
        "west": HUMAN_ROOM + "pine_st_w1.c",
        "east": HUMAN_ROOM + "pine_st_e1.c",
        "south": HUMAN_ROOM + "broad_st_s7.c",
    ]));
    set_looks(([
        "street": "A street sign is labeled S. Broad St.",
        ({ "framework", "hexagons"}): "Far above, the hexagons form a geodesic dome.",
    ]));
}