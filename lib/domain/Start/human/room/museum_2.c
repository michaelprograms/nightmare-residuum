#include "human.h"

inherit STD_ROOM;

void create () {
    ::create();
    set_properties(([ "indoors": 1, ]));
    set_short("a balcony over a lobby");
    set_long("Above the entrance to a building consisting of a spacious lobby. The ceilings are high above the polished floors below. The walls are made of stone blocks and covered with some tapestries. A grand staircase leads down towards the lobby below. Hallways branch to the east and south, heading to the wings of the building.");
    set_exits(([
        "east": HUMAN_ROOM + "museum_2_e1.c",
        "south": HUMAN_ROOM + "museum_2_s1.c",
        "down": HUMAN_ROOM + "museum_1.c",
    ]));
    set_looks(([
        ({ "building", "ceiling", "walls" }): "A building of large proportions. The lobby is a large room with high ceilings and distant walls.",
        "floor": "The floor is a polished white stone in large blocks.",
        "staircase": "The staircase is in the back of the lobby. The steps are wide stones with a short wall as a railing.",
        ({ "hallways", "wings" }): "Two hallways branch off from the main lobby, leading to the wings of the building.",
    ]));
}