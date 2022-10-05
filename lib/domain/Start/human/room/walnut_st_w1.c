#include "human.h"

inherit STD_ROOM;

void create () {
    ::create();
    set_short("a village street");
    set_long("A small street traveling west to east through brick dwellings covered in dark ivy. Several birch trees branch over the street and dwellings. A wide building that resembles an athletic training center on the north side of the street has its door open. A framework of hexagons glimmer overhead.");

    set_exits(([
        "east": HUMAN_ROOM + "broad_st_s3.c",
        "west": HUMAN_ROOM + "walnut_st_w2.c",
        "enter north": HUMAN_ROOM + "intro_warrior.c",
    ]));
    set_looks(([
        "street": "A village street labeled W Walnut St.",
        "building": "The building looks to be a warrior hall.",
    ]));
    set_reset(([
        HUMAN_NPC + "teen_fighter.c": 2,
    ]));
}