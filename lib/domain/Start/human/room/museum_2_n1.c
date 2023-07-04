#include "human.h"

inherit STD_ROOM;

void create () {
    ::create();
    set_properties(([ "indoors": 1, "no attack": 1, ]));
    set_short("a historical building");
    set_long("The northern hallway of a building with high ceilings and polished floors. The walls are made of stone blocks and covered with tapestries. The hallway turns towards the north as it heads further into the building.");
    set_exits(([
        "south": HUMAN_ROOM + "museum_2.c",
        "north": HUMAN_ROOM + "museum_2_n2.c",
    ]));
    set_looks(([

    ]));
    set_reset(([
        HUMAN_ITEM + "sculpture_1.c": 1,
    ]));
}