#include "human.h"

inherit STD_ROOM;

void create () {
    ::create();
    set_short("an orchard");
    set_long("Lines of evenly spaced trees form an orchard. Grassy pathways with dirt patches separate the rows. A framework of hexagons glimmer overhead.");
    set_exits(([
        "west": HUMAN_ROOM + "orchard_6.c",
        "south": HUMAN_ROOM + "orchard_4.c",
    ]));
    set_looks(([

    ]));
    set_reset(([
        HUMAN_ITEM + "apple_tree.c": 1,
        HUMAN_MONSTER + "ant.c": !random(3),
        HUMAN_MONSTER + "bee.c": !random(3),
        HUMAN_MONSTER + "beetle.c": !random(3),
        HUMAN_MONSTER + "fly.c": !random(3),
        HUMAN_MONSTER + "squirrel.c": !random(3),
        HUMAN_MONSTER + "termite.c": !random(3),
        HUMAN_MONSTER + "wasp.c": !random(3),
    ]));
}