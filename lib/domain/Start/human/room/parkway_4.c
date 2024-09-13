#include <human.h>

inherit STD_ROOM;

void create () {
    ::create();
    set_short("a village street before a building");
    set_long("A street traveling southeast with green spaces along the side. A large building borders on the west and north, with an entrance to the northwest. An obelisk can be seen to the southeast. A framework of hexagons glimmer far overhead.");
    set_listen("default", "Birds chirp from the trees.");
    set_smell("default", "Fresh air smelling of grass.");
    set_exits(([
        "enter northwest": HUMAN_ROOM + "museum_1.c",
        "southeast": HUMAN_ROOM + "park_nw.c",
    ]));
    set_looks(([
        "street": "A street sign is labeled Parkway.",
        ({ "framework", "hexagons"}): "Far above, the hexagons form a geodesic dome.",
    ]));
    set_reset(([
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