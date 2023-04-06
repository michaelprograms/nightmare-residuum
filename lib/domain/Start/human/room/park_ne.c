#include "human.h"

inherit STD_ROOM;

void create () {
    ::create();
    set_short("a village park");
    set_long("Amongst a village park where two bisecting streets blend into a green space. A pathway meanders through the park's grassy areas, with some sections growing freely into a meadow filled with wild grass and flower. A framework of hexagons glimmer far overhead.");
    set_listen("default", "Water softly splashes into the fountain basin.");
    set_smell("default", "Fresh air smelling of grass.");
    set_exits(([
        "west": HUMAN_ROOM + "chestnut_st_e2.c",
        "east": HUMAN_ROOM + "chestnut_st_e4.c",
        "north": HUMAN_ROOM + "fourth_st_n4.c",
        "south": HUMAN_ROOM + "fourth_st_n2.c",
    ]));
    set_looks(([
        "street": "The street signs are labeled Fourth St and Chestnut St.",
        ({ "framework", "hexagons"}): "Far above, the hexagons form a geodesic dome.",
    ]));
    set_reset(([
        HUMAN_ITEM + "bench.c": 1,
        HUMAN_ITEM + "fountain.c": 1,
        HUMAN_NPC + "ant.c": !random(3),
        HUMAN_NPC + "bee.c": !random(3),
        HUMAN_NPC + "beetle.c": !random(3),
        HUMAN_NPC + "fly.c": !random(3),
        HUMAN_NPC + "hamster.c": 1,
        HUMAN_NPC + "squirrel.c": !random(3),
        HUMAN_NPC + "termite.c": !random(3),
        HUMAN_NPC + "wasp.c": !random(3),
        HUMAN_NPC + "pigeon.c": !random(2),
    ]));
}