#include "human.h"

inherit "/std/room/class_hall.c";

void create () {
    ::create();
    set_properties(([
        "no attack": 1,
    ]));
    set_short("a village pub");
    set_long("The inside of a brick building that consists of a sparsely furnished room. There is an open door to the south facing the street next to a singular wide window. Some chairs are scattered around some weight training equipment.");
    set_exits(([
        "down": HUMAN_ROOM + "intro_warrior.c",
    ]));
    set_looks(([
        ({ "chairs", "equipment" }): "Chairs in this sparsely furnished room that are scattered around a few weight training stations.",
        ({ "door", "window" }): "The front of the building consists of a door that swings open to the street and a wide window.",
    ]));
    set_class("warrior");
}
