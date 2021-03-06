#include "human.h"

inherit "/std/room/class_hall.c";

void create () {
    ::create();
    set_properties(([
        "no attack": 1,
    ]));
    set_short("a warrior's hall");
    set_long("The second floor of a brick building that consists of the personal training area of the warriors. Training equipment can be seen scattered around the sides of the room. A staircase leads back down to the first floor.");
    set_exits(([
        "down": HUMAN_ROOM + "intro_warrior.c",
    ]));
    set_looks(([
        "equipment": "Things used to make a warrior strong.",
        "stairs": "The staircase lead back down to the first floor of the building.",
    ]));
    set_class("warrior");
}
