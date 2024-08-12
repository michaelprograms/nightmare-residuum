#include <human.h>

inherit STD_ROOM;

void create () {
    ::create();
    set_short("a village avenue");
    set_long("A narrow avenue traveling northeast to southwest with many buildings clustered around. A building with a glass window is on the southeast side of the street. A small store is on the northwest side of the street. A framework of hexagons glimmer far overhead.");
    set_exits(([
        "northeast": HUMAN_ROOM + "avenue_n2.c",
        "southwest": HUMAN_ROOM + "park_se.c",
        "enter southeast": HUMAN_ROOM + "store_clothing.c",
        "enter northwest": HUMAN_ROOM + "store_general.c",
    ]));
    set_looks(([
        "street": "A street sign is labeled 100 N. Avenue.",
        ({ "framework", "hexagons"}): "Far above, the hexagons form a geodesic dome.",
    ]));
}