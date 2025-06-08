#include <human.h>

inherit STD_ROOM;

void create () {
    ::create();
    set_short("a village street");
    set_long("A small street traveling north to south through brick dwellings covered in dark ivy. Several birch trees branch over the street and dwellings. A building with open doors on the east side of the street. A public square can be seen to the south surrounding an obelisk. A framework of hexagons glimmer overhead.");
    set_exits(([
        "north": HUMAN_ROOM + "broad_st_n2.c",
        "enter east": HUMAN_ROOM + "building_office.c",
        "south": HUMAN_ROOM + "square.c",
    ]));
    set_looks(([
        "street": "A street sign is labeled N. Broad St.",
        ({ "building", "doors" }): "The open doors leads into a wide but short building.",
        "banner": "The banner simply says Adventurer Hall.",
        "square": "A square plaza with trees, benches, and more.",
        ({ "framework", "hexagons"}): "Far above, the hexagons form a geodesic dome.",
    ]));
    set_reset(([

    ]));
}