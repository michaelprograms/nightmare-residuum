#include "human.h"

inherit STD_ROOM;

void create () {
    ::create();
    set_short("a spacious lobby");
    set_long("The entrance to a building consisting of a spacious lobby with high ceilings and polished floors. The walls are made of stone blocks and covered with some tapestries. A grand staircase along the back of the lobby leads up towards another floor that looks down upon the lobby.");
    set_exits(([
        "southeast": HUMAN_ROOM + "parkway_4.c",
        "east": HUMAN_ROOM + "museum_e1.c",
        "south": HUMAN_ROOM + "museum_w1.c",
        "up": HUMAN_ROOM + "museum_2.c",
    ]));
    set_looks(([
        ({ "building", "ceiling", "walls" }): "A building of large proportions. The lobby is a large room with high ceilings and distant walls.",
        "floor": "The floor is a polished white stone in large blocks.",
        "staircase": "The staircase is in the back of the lobby. The steps are wide stones with a short wall as a railing.",
    ]));
}