#include <human.h>

inherit STD_ROOM;

void create () {
    ::create();
    set_short("a village avenue");
    set_long("A narrow avenue traveling northeast to southwest with many buildings clustered around. A framework of hexagons glimmer far overhead.");
    set_exits(([
        "northeast": HUMAN_ROOM + "park_se.c",
        "southwest": HUMAN_ROOM + "avenue_s2.c",
    ]));
    set_looks(([
        "street": "A street sign is labeled 100 S. Avenue.",
        ({ "framework", "hexagons"}): "Far above, the hexagons form a geodesic dome.",
    ]));
}