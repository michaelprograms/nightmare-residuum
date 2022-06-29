#include "human.h"

inherit STD_ROOM;

void create () {
    ::create();
    set_short("a grassy meadow");
    set_long("A meadow filled with wild grasses and flowers. The west side of the meadow abuts an enormous stone structure with a wide opening at its base. A path leads back to a village street. A framework of hexagons glimmer far overhead.");
    set_looks(([
        ({ "grasses", "flowers" }): "A simple variety of wild grasses and flowers.",
        "structure": "A large rock formation jutting out of the ground.",
        "hexagons": "Far above, the hexagons form a geodesic dome.",
        "street": "A narrow street that passes through a village.",
    ]));
    set_exits(([
        "southeast": HUMAN_ROOM + "broad_st_n2.c",
    ]));
    set_reset(([
        HUMAN_MONSTER + "hamster.c": 1,
    ]));
}