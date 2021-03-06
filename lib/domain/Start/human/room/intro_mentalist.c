#include "human.h"

inherit "/std/room/class_intro.c";

void create () {
    ::create();
    set_properties(([
        "no attack": 1,
    ]));
    set_short("a mentalist's hall");
    set_long("The inside of a wide brick building that consists of a large unfurnished room. There is an open door to the east facing the street next to a singular wide window. Meditation mats are scattered around the floor. A staircase leads up to the second floor.");
    set_exits(([
        "out east": HUMAN_ROOM + "pine_st_n1.c",
    ]));
    set_exit("up", HUMAN_ROOM + "hall_mentalist.c", (: handle_pre_exit :), 0);
    set_looks(([
        ({ "mats" }): "Small mats used by those attempting to meditate, despite your intrusion.",
        ({ "door", "window" }): "The front of the building consists of a door that swings open to the street and a wide window.",
        "stairs": "The staircase lead up to the second floor of the building, where the private training of mentalists takes place.",
    ]));

    set_class("mentalist");
}