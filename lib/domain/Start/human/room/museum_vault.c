#include "human.h"

inherit STD_ROOM;

void create () {
    ::create();
    set_properties(([ "indoors": 1, ]));
    set_short("a chamber inside a building");
    set_long("Inside of a room with bright lights shining down. The concrete floor looks old here and unpolished, with a circular vault entrance located in the center of the chamber. The walls are made of stone blocks. Archways lead out of the chamber to the east and south.");
    set_exits(([
        "south": HUMAN_ROOM + "museum_1_w1.c",
        "east": HUMAN_ROOM + "museum_1_n1.c",
    ]));
    set_looks(([
       "vault": "A circular vault exit sits in the center of the chamber's floor. It is sealed and there don't seem to be any controls on this side of the vault door.",
    ]));
}