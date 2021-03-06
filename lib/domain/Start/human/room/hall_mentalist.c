#include "human.h"

inherit "/std/room/class_hall.c";

void create () {
    ::create();
    set_properties(([
        "no attack": 1,
    ]));
    set_short("a mentalist's hall");
    set_long("The second floor of a brick building that consists of the personal training area of the mentalists. Meditation mats can be seen scattered around the open floor. A staircase leads back down to the first floor.");
    set_exits(([
        "down": HUMAN_ROOM + "intro_mentalist.c",
    ]));
    set_looks(([
        "mats": "Mats used to help a mentalist meditate.",
        "stairs": "The staircase lead back down to the first floor of the building.",
    ]));
    set_class("mentalist");
}
