#include "human.h"

inherit STD_ROOM;

void create () {
    ::create();
    set_properties(([ "indoors": 1, ]));
    set_short("a historical building");
    set_long("The eastern hallway of a building with high ceilings and polished floors. The walls are made of stone blocks and covered with some tapestries. The end of the hallway comes to a staircase leading up to the second floor.");
    set_exits(([
        "south": HUMAN_ROOM + "museum_2_e1.c",
        "down": HUMAN_ROOM + "museum_1_e2.c",
    ]));
    set_looks(([

    ]));
}