#include "human.h"

inherit STD_ROOM;

void create () {
    ::create();
    set_short("a village street");
    set_long("A small street traveling west to east through brick dwellings covered in dark ivy. Several birch trees branch over the street and dwellings. An vaulted building sits on the south side of the street with a large bell in a tower. A framework of hexagons glimmer overhead.");
    set_exits(([
        "east": HUMAN_ROOM + "market_st_e6.c",
        "west": HUMAN_ROOM + "market_st_e4.c",
        "enter south": HUMAN_ROOM + "intro_templar.c",
    ]));
    set_looks(([
        "street": "A village street labeled E Market St.",
    ]));
}