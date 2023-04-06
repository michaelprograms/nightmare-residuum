#include "human.h"

inherit STD_ROOM;

void create () {
    ::create();
    set_short("a village street");
    set_long("A small street traveling west to east through brick dwellings covered in dark ivy. Several birch trees branch over the street and dwellings. A framework of hexagons glimmer overhead.");
    set_exits(([
        "west": HUMAN_ROOM + "broad_st_s6.c",
        "east": HUMAN_ROOM + "pine_st_e2.c",
    ]));
    set_looks(([
        "street": "A street sign is labeled E. Pine St.",
        ({ "framework", "hexagons"}): "Far above, the hexagons form a geodesic dome.",
    ]));
    set_reset(([
        HUMAN_NPC + "teen_suspicious.c": 1,
    ]));
}