#include <human.h>

inherit STD_ROOM;

void create () {
    ::create();
    set_short("a village street");
    set_long("A small street traveling north to south through brick dwellings covered in dark ivy. Several birch trees branch over the street and dwellings. An open grassy area is to the south. A large marble building is to the west. A framework of hexagons glimmer overhead.");
    set_exits(([
        "north": HUMAN_ROOM + "second_st_n5.c",
        "south": HUMAN_ROOM + "park_nw.c",
    ]));
    set_looks(([
        "street": "A street sign is labeled N. Second St.",
        "building": "A large marble building that takes spans the width of the street block. It looks to be a couple stories tall with an ornate crowning around the roof line. Some windows are spaced out along the side of the building. Columns span the entranceway.",
        ({ "framework", "hexagons"}): "Far above, the hexagons form a geodesic dome.",
    ]));
}