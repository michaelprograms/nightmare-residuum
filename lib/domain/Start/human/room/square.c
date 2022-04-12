#include "human.h"

inherit STD_ROOM;

void create () {
    ::create();
    set_properties(([
        "no attack": 1,
    ]));
    set_short("a village crossroads");
    set_long("Amongst a village center where two central streets form a crossroads. A plaza of stone pavers blends the intersection into a public square. Brick dwellings covered in dark ivy form a cozy enclosure around the outer circumference of the plaza. Birch trees branch over the perimeter between the dwellings and the plaza. A framework of hexagons glimmer overhead.");
    set_looks(([
        ({ "center", "plaza", "square" }): "The central meeting point of the village, where most of the locals pass by during their day.",
        "dwellings": "Squat brick buildings of two floors in height. Modest windows with wooden shutters adorn their front.",
        ({ "birch", "trees" }): "Birch trees of various ages branch over the plaza's perimeter.",
        "hexagons": "Far above, the hexagons form a geodesic dome.",
        "streets": "The streets Broad and Market meet and cross here.",
    ]));
    set_exits(([
        "north": HUMAN_ROOM + "broad_st_n1.c",
        "east": HUMAN_ROOM + "market_st_e1.c",
        "south": HUMAN_ROOM + "broad_st_s1.c",
        "west": HUMAN_ROOM + "market_st_w1.c",
    ]));
    set_reset(([
        HUMAN_ITEM + "storage_bin.c": 1,
        HUMAN_ITEM + "obelisk.c": 1,
        HUMAN_ITEM + "plum_tree.c": 1,
        HUMAN_ITEM + "bench.c": 2,
        HUMAN_NPC + "clone.c": 5,
    ]));
}