#include "human.h"

inherit STD_ROOM;

void create () {
    ::create();
    set_short("an orchard entrance");
    set_long("A small alley leads west ends here as it opens up into a field behind some brick dwellings. A village street can be seen to the west. An orchard enclosed in a wooden fence spans to the east. A framework of hexagons glimmer overhead.");
    set_exits(([
        "west": HUMAN_ROOM + "orchard_alley.c",
        "east": HUMAN_ROOM + "orchard_1.c",
    ]));
    set_looks(([
        "street": "A village street down the alley the west.",
        "orchard": "The orchard of trees looks to contain several different types of trees.",
    ]));
}