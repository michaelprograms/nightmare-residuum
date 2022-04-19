#include "human.h"

inherit STD_ROOM;

void create () {
    ::create();
    set_short("an orchard entrance");
    set_long("A small alley traveling west and east through brick dwellings covered in dark ivy. A village street can be seen to the west. An ochard enclosed in a wooden fence spans to the east. A framework of hexagons glimmer overhead.");
    set_exits(([
        "west": HUMAN_ROOM + "broad_st_n3.c",
    ]));
    set_looks(([
        "street": "A village street down the alley the west.",
        "orchard": "The orchard of trees looks to contain several different types of trees.",
    ]));
}