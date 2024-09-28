#include <human.h>

inherit STD_ROOM;

void create () {
    ::create();
    set_properties(([ "indoors": 1, "no attack": 1, ]));
    set_short("a historical building");
    set_long("The western hallway of a building with high ceilings and polished floors. The walls are made of stone blocks and covered with tapestries. The hallway leads further west into the building or east to the lobby.");
    set_exits(([
        "west": HUMAN_ROOM + "museum_1_w2.c",
        "north": HUMAN_ROOM + "museum_1.c",
    ]));
    set_looks(([
        ({ "building", "ceiling", "walls" }): "A building of large proportions. This hallway is a large room with high ceilings and distant walls.",
        "floor": "The floor is a polished white stone in large blocks.",
        "lobby": "The entrance to the building.",
        "hallway": "It leads away and also towards the lobby of the building.",
    ]));
    set_reset(([
        HUMAN_NPC + "museum_visitor.c": 1,
    ]));
}