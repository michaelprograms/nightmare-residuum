#include "human.h"

inherit STD_ROOM;

void create () {
    ::create();
    set_short("a historical building");
    set_long("The eastern hallway of a building with high ceilings and polished floors. The walls are made of stone blocks and covered with some tapestries. The hallway turns towards the north as it heads further into the building.");
    set_exits(([
        "west": HUMAN_ROOM + "museum_2.c",
        "north": HUMAN_ROOM + "museum_2_e2.c",
    ]));
    set_looks(([

    ]));
}