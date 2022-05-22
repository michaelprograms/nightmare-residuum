#include "human.h"

inherit STD_ROOM;

void create () {
    ::create();
    set_short("a village street");
    set_long("A small street traveling north and south through brick dwellings covered in dark ivy. Several birch trees branch over the street and dwellings. An orchard can be seen down an alley heading towards the east. A framework of hexagons glimmer overhead.");
    set_exits(([
        "east": HUMAN_ROOM + "orchard_alley.c",
        "north": HUMAN_ROOM + "broad_st_n4.c",
        "south": HUMAN_ROOM + "broad_st_n2.c",
    ]));
    set_looks(([
        "street": "A village street labeled N Broad St.",
        "orchard": "An ochard of fruiting trees can be seen between a gap in the buildings to the east.",
    ]));
}