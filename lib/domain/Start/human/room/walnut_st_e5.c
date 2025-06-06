#include <human.h>

inherit STD_ROOM;

void create () {
    ::create();
    set_short("a village street");
    set_long("A small street traveling west to east through brick dwellings covered in dark ivy. Several birch trees branch over the street and dwellings. A framework of hexagons glimmer overhead.");
    set_exits(([
        "east": HUMAN_ROOM + "walnut_st_e6.c",
        "west": HUMAN_ROOM + "walnut_st_e4.c",
        "enter south": HUMAN_ROOM + "building_rundown2.c",
    ]));
    set_looks(([
        "street": "A street sign is labeled E. Walnut St.",
        ({ "framework", "hexagons"}): "Far above, the hexagons form a geodesic dome.",
    ]));
    set_reset(([
        HUMAN_NPC + "greasy_rat.c": 1,
    ]));
}