#include <human.h>

inherit STD_ROOM;

void create () {
    ::create();
    set_properties(([ "indoors": 1, ]));
    set_short("a historical building");
    set_long("The western hallway of a building with high ceilings and polished floors. The walls are made of stone blocks and covered with tapestries. The hallway turns towards the west as it heads further into the building.");
    set_exits(([
        "north": HUMAN_ROOM + "museum_2.c",
        "west": HUMAN_ROOM + "museum_2_w2.c",
    ]));
    set_looks(([
        ({ "building", "ceiling", "walls" }): "A building of large proportions. This hallway is a large room with high ceilings and distant walls.",
        "floor": "The floor is a polished white stone in large blocks.",
        "hallway": "It leads away and also towards the lobby of the building.",
    ]));
    set_reset(([
        HUMAN_ITEM + "sculpture_3.c": 1,
    ]));
}