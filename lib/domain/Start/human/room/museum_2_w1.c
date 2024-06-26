#include <human.h>

inherit STD_ROOM;

void create () {
    ::create();
    set_properties(([ "indoors": 1, "no attack": 1, ]));
    set_short("a historical building");
    set_long("The western hallway of a building with high ceilings and polished floors. The walls are made of stone blocks and covered with tapestries. The hallway turns towards the west as it heads further into the building.");
    set_exits(([
        "east": HUMAN_ROOM + "museum_2.c",
        "west": HUMAN_ROOM + "museum_2_w2.c",
    ]));
    set_looks(([

    ]));
}