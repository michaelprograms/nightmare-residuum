#include "human.h"

inherit STD_ROOM;

void create () {
    ::create();
    set_properties(([ "indoors": 1, ]));
    set_short("a historical building");
    set_long("The southern hallway of a building with high ceilings and polished floors. The walls are made of stone blocks and covered with some tapestries. The end of the hallway comes to a staircase leading up to the second floor.");
    set_exits(([
        "east": HUMAN_ROOM + "museum_1_s1.c",
        "up": HUMAN_ROOM + "museum_2_s2.c",
    ]));
    set_looks(([
        ({ "building", "ceiling", "walls" }): "A building of large proportions. This hallway is a large room with high ceilings and distant walls.",
        "floor": "The floor is a polished white stone in large blocks.",
        "hallway": "It leads away from the lobby and turns.",
    ]));
}