#include "human.h"

inherit STD_ROOM;

void create () {
    ::create();
    set_properties(([ "indoors": 1, ]));
    set_short("a historical building");
    set_long("The western hallway of a building with high ceilings and polished floors. The walls are made of stone blocks and covered with tapestries. The hallway leads further west into the building or east to the lobby. An archway leads to a chamber to the north.");
    set_exits(([
        "east": HUMAN_ROOM + "museum_1.c",
        "west": HUMAN_ROOM + "museum_1_w2.c",
        "north": HUMAN_ROOM + "museum_vault.c",
    ]));
    set_looks(([
        ({ "building", "ceiling", "walls" }): "A building of large proportions. This hallway is a large room with high ceilings and distant walls.",
        "floor": "The floor is a polished white stone in large blocks.",
        "lobby": "The entrance to the building.",
        "hallway": "It leads away from the lobby and turns.",
        ({ "archway", "chamber", }): "A chamber of some sort off of the hallway through an archway.",
    ]));
}