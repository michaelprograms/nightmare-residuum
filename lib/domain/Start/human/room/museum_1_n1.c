#include <human.h>

inherit STD_ROOM;

void create () {
    ::create();
    set_properties(([ "indoors": 1, ]));
    set_short("a historical building");
    set_long("The northern hallway of a building with high ceilings and polished floors. The walls are made of stone blocks and covered with tapestries. The hallway leads further north into the building or south to the lobby. An archway leads to a chamber to the west.");
    set_exits(([
        "north": HUMAN_ROOM + "museum_1_n2.c",
        "west": HUMAN_ROOM + "museum_1.c",
    ]));
    set_looks(([
        ({ "building", "ceiling", "walls" }): "A building of large proportions. This hallway is a large room with high ceilings and distant walls.",
        "floor": "The floor is a polished white stone in large blocks.",
        "lobby": "The entrance to the building.",
        "hallway": "It leads away and also towards the lobby of the building.",
        ({ "archway", "chamber", }): "A chamber of some sort off of the hallway through an archway.",
    ]));
    set_reset(([
        HUMAN_NPC + "museum_attendant.c": 1,
        HUMAN_ITEM + "sculpture_1.c": 1,
    ]));
}