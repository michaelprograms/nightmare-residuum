#include "human.h"

inherit STD_ROOM;

void create () {
    ::create();
    set_short("a village park");
    set_long("Amongst a village park where two bisecting streets blend into a green space. A pathway meanders through the park's grassy areas, with some sections growing freely into a meadow filled with wild grass and flower. A framework of hexagons glimmer far overhead.");
    set_listen("default", "Water softly splashes into the fountain basin.");
    set_smell("default", "Fresh air smelling of grass.");
    set_exits(([
        "west": HUMAN_ROOM + "walnut_st_w4.c",
        "east": HUMAN_ROOM + "walnut_st_w2.c",
        "north": HUMAN_ROOM + "second_st_s2.c",
        "south": HUMAN_ROOM + "second_st_s4.c",
    ]));
    set_looks(([
        "street": "The street signs are labeled Second St and Walnut St.",
        ({ "framework", "hexagons"}): "Far above, the hexagons form a geodesic dome.",
    ]));
    set_reset(([
        HUMAN_ITEM + "bench.c": 1,
        HUMAN_ITEM + "fountain.c": 1,
        HUMAN_NPC + "squirrel_person.c": 1,
        HUMAN_NPC + "ant.c": ([
            "number": !random(3),
        ]),
        HUMAN_NPC + "bee.c": ([
            "number": !random(3),
        ]),
        HUMAN_NPC + "beetle.c": ([
            "number": !random(3),
        ]),
        HUMAN_NPC + "squirrel.c": ([
            "number": !random(3),
        ]),
        HUMAN_NPC + "wasp.c": ([
            "number": !random(3),
        ]),
        HUMAN_NPC + "pigeon.c": ([
            "number": !random(2),
        ]),
        HUMAN_NPC + "sparrow.c": ([
            "number": !random(2),
        ]),
    ]));
}