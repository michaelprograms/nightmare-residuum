#include "human.h"

inherit STD_ROOM;

void create () {
    ::create();
    set_properties(([
        "no attack": 1,
        "indoors": 1,
    ]));
    set_short("a warrior's hall");
    set_long("The inside of a wide brick building that consists of a sparsely furnished room. There is an open door to the south facing the street next to a singular wide window. Some chairs are scattered around some weight training equipment. A staircase leads up to the second floor.");
    set_listen("default", "Several warriors talk boisterously while lifting weights.");
    set_smell("default", "The smell of sweat.");
    set_exits(([
        "out south": HUMAN_ROOM + "walnut_st_w1.c",
        "up": HUMAN_ROOM + "hall_warrior.c",
    ]));
    set_looks(([
        ({ "chairs", "equipment" }): "Chairs in this sparsely furnished room that are scattered around a few weight training stations.",
        ({ "door", "window" }): "The front of the building consists of a door that swings open to the street and a wide window.",
        "stairs": "The wooden staircase leads up to the second floor of the building, where the warriors like to train away from prying eyes.",
    ]));
}