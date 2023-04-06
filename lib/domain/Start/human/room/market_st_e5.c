#include "human.h"

inherit STD_ROOM;

void create () {
    ::create();
    set_short("a village street");
    set_long("A small street traveling west to east through brick dwellings covered in dark ivy. Several birch trees branch over the street and dwellings. A wide vaulted building sits on the south side of the street with bell-tower. A framework of hexagons glimmer overhead.");
    set_listen("default", "Chanting of hymns can be heard coming from a building to the south.");
    set_smell("default", "A light floral scent lingers around the south side of the street.");
    set_exits(([
        "east": HUMAN_ROOM + "market_st_e6.c",
        "west": HUMAN_ROOM + "market_st_e4.c",
        "enter south": HUMAN_ROOM + "intro_mystic.c",
    ]));
    set_looks(([
        "street": "A street sign is labeled E. Market St.",
        "building": "The building looks to be a mystic hall.",
        ({ "framework", "hexagons"}): "Far above, the hexagons form a geodesic dome.",
    ]));
}