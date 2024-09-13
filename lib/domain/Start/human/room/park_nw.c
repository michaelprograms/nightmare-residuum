#include <human.h>

inherit STD_ROOM;

void create () {
    ::create();
    set_short("a village park");
    set_long("Amongst a village park where two bisecting streets blend into a green space, with a third street traveling from the southeast and ending here. A large marble building with columns surrounding the entrance sits to the northwest. A pathway meanders through the park's grassy areas, with some sections growing freely into a meadow filled with wild grass and flower. An obelisk can be seen to the southeast. A framework of hexagons glimmer far overhead.");
    set_listen("default", "Water loudly splashes into the fountain basin.");
    set_smell("default", "Fresh air smelling of grass.");
    set_exits(([
        "northwest": HUMAN_ROOM + "parkway_4.c",
        "west": HUMAN_ROOM + "chestnut_st_w4.c",
        "east": HUMAN_ROOM + "chestnut_st_w2.c",
        "north": HUMAN_ROOM + "second_st_n4.c",
        "south": HUMAN_ROOM + "second_st_n2.c",
        "southeast": HUMAN_ROOM + "parkway_2.c",
    ]));
    set_looks(([
        "street": "The street signs are labeled Second St, Chestnut St, and Parkway.",
        ({ "building", "columns", }): "A large marble building that takes spans the width of the street block. It looks to be a couple stories tall with an ornate crowning around the roof line. Some windows are spaced out along the side of the building. Columns span the entranceway.",
        ({ "framework", "hexagons"}): "Far above, the hexagons form a geodesic dome.",
    ]));
    set_reset(([
        HUMAN_ITEM + "bench.c": 1,
        HUMAN_ITEM + "grand_fountain.c": 1,
        HUMAN_ITEM + "artwork_3.c": 1,
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