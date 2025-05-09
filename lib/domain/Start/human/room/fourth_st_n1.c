#include <human.h>

inherit STD_ROOM;

void create () {
    ::create();
    set_short("a village street");
    set_long("A small street traveling north to south through brick dwellings covered in dark ivy. Several birch trees branch over the street and dwellings. A wide building resembling a temple of worship on the west side of the street has its door open. A framework of hexagons glimmer overhead.");
    set_listen("default", "The soft sounds of chanting occasionally carry through the air.");
    set_smell("default", "A light incense wafts through the air.");
    set_exits(([
        "south": HUMAN_ROOM + "market_st_e3.c",
        "north": HUMAN_ROOM + "fourth_st_n2.c",
        "enter west": HUMAN_ROOM + "building_temple1.c",
    ]));
    set_looks(([
        "street": "A street sign is labeled N. Fourth St.",
        "building": "The building looks to be a paladin hall.",
        ({ "framework", "hexagons"}): "Far above, the hexagons form a geodesic dome.",
    ]));
}