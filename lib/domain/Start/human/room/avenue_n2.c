#include <human.h>

inherit STD_ROOM;

void create () {
    ::create();
    set_short("a rrow village avenue");
    set_long("A narrow avenue traveling northeast to southwest with many buildings clustered around. A framework of hexagons glimmer far overhead.");
    set_exits(([
        "northeast": HUMAN_ROOM + "market_st_e6.c",
        "southwest": HUMAN_ROOM + "avenue_n1.c",
    ]));
    set_looks(([
        "street": "A street sign is labeled 200 N. Avenue.",
        ({ "framework", "hexagons"}): "Far above, the hexagons form a geodesic dome.",
    ]));
    set_reset(([
        HUMAN_NPC + "teen_suspicious.c": 1,
    ]));
}