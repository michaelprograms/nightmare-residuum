#include "human.h"

inherit STD_ROOM;

void create () {
    ::create();
    set_short("a village street");
    set_long("A small street traveling south through brick dwellings covered in dark ivy. Several birch trees branch over the street and dwellings. A wide brick building with an arched, gated entrance takes up on most of the west side of the street. A framework of hexagons glimmer overhead. A concrete wall at the base of the hexagon dome blocks further passage north.");
    set_listen("default", "Occasionally several faint thuds can be heard from the west.");
    set_smell("default", "A wiff of wildflowers comes from somewhere.");
    set_exits(([
        "south": HUMAN_ROOM + "pine_st_n5.c",
        "enter west": HUMAN_ROOM + "intro_ranger.c",
    ]));
    set_looks(([
        "street": "A village street labeled N Pine St.",
        "building": "The building looks to be a ranger hall.",
    ]));
}