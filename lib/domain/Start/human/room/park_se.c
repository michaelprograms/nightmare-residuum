#include <human.h>

inherit STD_ROOM;

void create () {
    ::create();
    set_short("a village park");
    set_long("Amongst a village park where three bisecting streets blend into a green space. A pathway meanders through the park's grassy areas, with some sections growing freely into a meadow filled with wild grass and flower. Two restaurants can be seen on opposite sides of the park. A framework of hexagons glimmer far overhead.");
    set_listen("default", "Water softly splashes into the fountain basin.");
    set_smell("default", "The smell of cooked foods from nearby restaurants.");
    set_exits(([
        "northeast": HUMAN_ROOM + "avenue_n1.c",
        "southwest": HUMAN_ROOM + "avenue_s1.c",
        "west": HUMAN_ROOM + "walnut_st_e2.c",
        "east": HUMAN_ROOM + "walnut_st_e4.c",
        "north": HUMAN_ROOM + "fourth_st_s2.c",
        "south": HUMAN_ROOM + "fourth_st_s4.c",
        "enter northwest": HUMAN_ROOM + "store_meatball.c",
        "enter southeast": HUMAN_ROOM + "store_falafel.c",
    ]));
    set_looks(([
        "street": "The street signs are labeled Fourth St, Walnut St, and Avenue.",
        ({ "framework", "hexagons"}): "Far above, the hexagons form a geodesic dome.",
    ]));
    set_reset(([
        HUMAN_ITEM + "bench.c": 1,
        HUMAN_ITEM + "fountain.c": 1,
        HUMAN_NPC + "teen_suspicious.c": ([
            "number": !random(2),
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