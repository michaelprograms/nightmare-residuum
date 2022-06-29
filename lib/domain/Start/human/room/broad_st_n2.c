#include "human.h"

inherit STD_ROOM;

void create () {
    ::create();
    set_short("a village street");
    set_long("A small street traveling north and south through brick dwellings covered in dark ivy. Several birch trees branch over the street and dwellings. A grassy meadow is to the northwest. A framework of hexagons glimmer overhead.");
    set_exits(([
        "north": HUMAN_ROOM + "broad_st_n3.c",
        "south": HUMAN_ROOM + "broad_st_n1.c",
    ]));
    set_looks(([
        "street": "A village street labeled N Broad St.",
        "meadow": "A meadow filled with grass behind a row of brick dwellings.",
    ]));
}