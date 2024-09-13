#include <human.h>

inherit STD_ROOM;

void create () {
    ::create();
    set_properties(([ "indoors": 1, "no attack": 1, ]));
    set_short("a historical building");
    set_long("The northern hallway of a building with high ceilings and polished floors. The walls are made of stone blocks and covered with tapestries, except for the western wall which consists of a large alcove containing a sculpture. The hallway turns towards the north as it heads further into the building.");
    set_exits(([
        "west": HUMAN_ROOM + "museum_2.c",
        "north": HUMAN_ROOM + "museum_2_n2.c",
    ]));
    set_looks(([
        ({ "building", "ceiling", "walls" }): "A building of large proportions. This hallway is a large room with high ceilings and distant walls.",
        "floor": "The floor is a polished white stone in large blocks.",
        "hallway": "It leads away and also towards the lobby of the building.",
        ({ "wall", "alcove" }): "A large alcove containing a sculpture covers nearly all of the western wall.",
    ]));
    set_reset(([
        HUMAN_ITEM + "sculpture_1.c": 1,
    ]));
}