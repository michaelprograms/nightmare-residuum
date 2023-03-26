#include "human.h"

inherit ROOM_CLASS_HALL;

void create () {
    ::create();
    set_properties(([
        "no attack": 1,
        "indoors": 1,
    ]));
    set_short("a warrior's hall");
    set_long("The second floor of a brick building that consists of the personal training area of the warriors. Training equipment can be seen scattered around the sides of the room. A staircase leads back down to the first floor.");
    set_listen("default", "Several warriors talk boisterously while lifting weights.");
    set_smell("default", "The warm smell of training bodies.");
    set_exits(([
        "down": HUMAN_ROOM + "intro_warrior.c",
    ]));
    set_looks(([
        "equipment": "Things used to make a warrior strong.",
        "stairs": "The staircase lead back down to the first floor of the building.",
    ]));
    set_class("warrior");
}
