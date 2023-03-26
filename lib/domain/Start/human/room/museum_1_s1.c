#include "human.h"

inherit STD_ROOM;

void create () {
    ::create();
    set_properties(([ "indoors": 1, ]));
    set_short("a historical building");
    set_long("The southern hallway of a building with high ceilings and polished floors. The walls are made of stone blocks and covered with some tapestries. The hallway turns towards the west as it heads further into the building. The lobby is to the north.");
    set_exits(([
        "north": HUMAN_ROOM + "museum_1.c",
        "west": HUMAN_ROOM + "museum_1_s2.c",
    ]));
    set_looks(([
        ({ "building", "ceiling", "walls" }): "A building of large proportions. This hallway is a large room with high ceilings and distant walls.",
        "floor": "The floor is a polished white stone in large blocks.",
        "lobby": "The entrance to the building.",
        "hallway": "It leads away from the lobby and turns.",
    ]));
}