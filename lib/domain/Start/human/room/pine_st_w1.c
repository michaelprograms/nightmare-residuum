#include <human.h>

inherit STD_ROOM;

void create () {
    ::create();
    set_short("a village street");
    set_long("A small street traveling west to east through brick dwellings covered in dark ivy. Several birch trees branch over the street and dwellings. A building can be seen to the south, which looks to be used as a hospital. A framework of hexagons glimmer overhead.");
    set_exits(([
        "west": HUMAN_ROOM + "pine_st_w2.c",
        "east": HUMAN_ROOM + "broad_st_s6.c",
        "enter south": HUMAN_ROOM + "building_hospital.c",
    ]));
    set_looks(([
        "street": "A street sign is labeled W. Pine St.",
        ({ "framework", "hexagons"}): "Far above, the hexagons form a geodesic dome.",
        "building": "A fairly large building compared to the neighboring buildings. It looks to be a medical center of some sorts.",
    ]));
}