#include <human.h>

inherit STD_ROOM;

void create () {
    ::create();
    set_short("a village street");
    set_long("A small street traveling west to east through brick dwellings covered in dark ivy. Several birch trees branch over the street and dwellings. A wide vaulted building sits on the south side of the street with bell-tower, while a large building with a yard sits to the north. A framework of hexagons glimmer overhead.");
    set_listen("default", "Chanting of hymns can be heard coming from a building to the south.");
    set_smell("default", "A light floral scent lingers around the south side of the street.");
    set_exits(([
        "east": HUMAN_ROOM + "market_st_e6.c",
        "west": HUMAN_ROOM + "market_st_e4.c",
        "enter south": HUMAN_ROOM + "building_church.c",
        "enter north": HUMAN_ROOM + "building_school1.c",
    ]));
    set_looks(([
        "street": "A street sign is labeled E. Market St.",
        "building": "A building to the south looks to be a church, while the the building to the south looks to be a school, with a yard adjacent to it.",
        ({ "framework", "hexagons"}): "Far above, the hexagons form a geodesic dome.",
    ]));
}