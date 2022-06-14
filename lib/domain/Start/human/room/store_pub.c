#include "human.h"

inherit STD_ROOM;

void create () {
    ::create();
    set_properties(([
        "no attack": 1,
    ]));
    set_short("a village pub");
    set_long("The inside of a brick building that consists of an open room. There are several open doors to the east facing the street with benches out front. Several tall chairs are placed around the inside in front of a large counter with a large flat surface. Various bottles are stored on some shelves on the wall behind the counter.");
    set_exits(([
        "out east": HUMAN_ROOM + "broad_st_s1.c",
    ]));
    set_looks(([
        ({ "bottles", "shelves" }): "Some bottles of liquid with varying levels of opacity. They look to be alcoholic.",
        "chairs": "Chairs in front of the counter for customers to be seated while being served.",
        "benches": "Some benches are placed in front of the building for patrons to sit outside.",
        "counter": "A wooden counter along the side of the room and allowing more room for patrons to enter and sit.",
        "doors": "The front of the building consists of several doors that swing open and open the building up to the street.",
    ]));
    set_reset(([
        // HUMAN_NPC + "vendor_barkeep.c": 1,
    ]));
}
