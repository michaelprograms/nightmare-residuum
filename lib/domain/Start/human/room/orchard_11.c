#include "human.h"

inherit STD_ROOM;

void create () {
    ::create();
    set_short("an orchard");
    set_long("Lines of evenly spaced trees form an orchard. Grassy pathways with dirt patches separate the rows. A framework of hexagons glimmer overhead. A concrete wall supporting the hexagon dome stands at the edge of town.");
    set_exits(([
        "southwest": HUMAN_ROOM + "orchard_6.c",
    ]));
    set_looks(([

    ]));
    set_reset(([
        HUMAN_ITEM + "peach_tree.c": 1,
        HUMAN_MONSTER + "ant.c": !random(3),
        HUMAN_MONSTER + "bee.c": !random(3),
        HUMAN_MONSTER + "beetle.c": !random(3),
        HUMAN_MONSTER + "fly.c": !random(3),
        HUMAN_MONSTER + "squirrel.c": !random(3),
        HUMAN_MONSTER + "termite.c": !random(3),
        HUMAN_MONSTER + "wasp.c": !random(3),
    ]));
}