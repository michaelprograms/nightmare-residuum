#include "human.h"

inherit STD_ROOM;

void create () {
    ::create();
    set_short("a village street");
    set_long("A small street traveling north to south through brick dwellings covered in dark ivy. Several birch trees branch over the street and dwellings. A building on the northeast corner of the intersection has benches set up and large open windows. A framework of hexagons glimmer overhead.");
    set_listen("default", "The chattering of a small crowd in and outside the pub.");
    set_smell("default", "Fried food from the pub fills the air.");
    set_exits(([
        "north": HUMAN_ROOM + "broad_st_s2.c",
        "south": HUMAN_ROOM + "broad_st_s4.c",
        "west": HUMAN_ROOM + "walnut_st_w1.c",
        "east": HUMAN_ROOM + "walnut_st_e1.c",
        "enter northeast": HUMAN_ROOM + "store_pub.c",
    ]));
    set_looks(([
        "street": "A village street labeled S Broad St.",
    ]));
    set_reset(([
        HUMAN_ITEM + "sign_pub.c": 1,
    ]));
}