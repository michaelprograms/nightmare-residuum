#include "human.h"

inherit STD_ROOM;

void create () {
    ::create();
    set_short("a village street");
    set_long("A small street traveling north to south through brick dwellings covered in dark ivy. Several birch trees branch over the street and dwellings. A building with a series of tall and wide windows with an open doorway is on the west side of the street. A framework of hexagons glimmer overhead.");
    set_listen("default", "This street is particularly quiet.");
    set_smell("default", "Nothing particularly noticable.");
    set_exits(([
        "south": HUMAN_ROOM + "market_st_w3.c",
        "north": HUMAN_ROOM + "second_st_n2.c",
        "enter west": HUMAN_ROOM "intro_psionist.c",
    ]));
    set_looks(([
        "street": "A street sign is labeled N. Second St.",
        "building": "The building looks to be a psionist hall.",
        ({ "framework", "hexagons"}): "Far above, the hexagons form a geodesic dome.",
    ]));
    set_reset(([
        HUMAN_NPC + "teen_meditate.c": 2,
    ]));
}