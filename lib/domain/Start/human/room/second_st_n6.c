#include <human.h>

inherit STD_ROOM;

void create () {
    ::create();
    set_short("a village street");
    set_long("A small street traveling south through brick dwellings covered in dark ivy. Several birch trees branch over the street and dwellings. A wide brick building with an arched, gated entrance takes up on most of the west side of the street. A large marble building is to the west-southwest. A concrete wall at the base of the hexagon dome blocks further passage north. A framework of hexagons glimmer overhead.");
    set_listen("default", "Occasionally several faint thuds can be heard from the west.");
    set_smell("default", "A wiff of wildflowers comes from somewhere.");
    set_exits(([
        "east": HUMAN_ROOM + "spruce_st_w2.c",
        "south": HUMAN_ROOM + "second_st_n5.c",
        "enter west": HUMAN_ROOM + "intro_ranger.c",
    ]));
    set_looks(([
        "street": "A street sign is labeled N. Second St.",
        "building": "A large marble building that takes spans the width of the street block. It looks to be a couple stories tall with an ornate crowning around the roof line. Some windows are spaced out along the side of the building. Columns span the entranceway.",
        "building": "The building looks to be a ranger hall.",
        ({ "framework", "hexagons"}): "Far above, the hexagons form a geodesic dome.",
    ]));
}