#include "human.h"

inherit STD_ROOM;

void create () {
    ::create();
    set_short("a village park");
    set_long("A grassy area with several lazy walkways passing through. Some sections of the area grow freely in a meadow filled with wild grasses and flowers. A framework of hexagons glimmer far overhead.");
    set_exits(([
        "west": HUMAN_ROOM + "walnut_st_w4.c",
        "east": HUMAN_ROOM + "walnut_st_w2.c",
        "north": HUMAN_ROOM + "pine_st_s2.c",
        "south": HUMAN_ROOM + "pine_st_s4.c",
    ]));
    set_looks(([

    ]));
    set_reset(([
        HUMAN_MONSTER + "ant.c": !random(3),
        HUMAN_MONSTER + "bee.c": !random(3),
        HUMAN_MONSTER + "beetle.c": !random(3),
        HUMAN_MONSTER + "fly.c": !random(3),
        HUMAN_MONSTER + "hamster.c": 1,
        HUMAN_MONSTER + "squirrel.c": !random(3),
        HUMAN_MONSTER + "termite.c": !random(3),
        HUMAN_MONSTER + "wasp.c": !random(3),
    ]));
}