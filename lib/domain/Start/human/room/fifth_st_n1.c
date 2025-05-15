#include <human.h>

inherit STD_ROOM;

void create () {
    ::create();
    set_short("a village street");
    set_long("A small street traveling north to south through brick dwellings covered in dark ivy. Several birch trees branch over the street and dwellings. A large building with a yard sits to the west. A framework of hexagons glimmer overhead.");
    set_exits(([
        "south": HUMAN_ROOM + "market_st_e6.c",
        "north": HUMAN_ROOM + "fifth_st_n2.c",
        "enter west": HUMAN_ROOM + "building_school1.c",
    ]));
    set_looks(([
        "street": "A street sign is labeled N. Fifth St.",
        ({ "framework", "hexagons"}): "Far above, the hexagons form a geodesic dome.",
    ]));
}