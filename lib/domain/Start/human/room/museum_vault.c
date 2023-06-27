#include "human.h"

inherit STD_ROOM;

void create () {
    ::create();
    set_properties(([ "indoors": 1, "no attack": 1, ]));
    set_short("a chamber inside a building");
    set_long("Inside of a room with bright lights shining down. The concrete floor looks old here and unpolished, with a circular vault entrance located in the center of the chamber. The walls are made of stone blocks and covered with tapestries. An archway leads out of the chamber into a hallway to the east.");
    set_exits(([
        "east": HUMAN_ROOM + "museum_1_n1.c",
    ]));
    set_looks(([
       ({ "vault", "floor", }): "A circular vault exit sits in the center of the chamber's floor. It is sealed and there are no visible controls on the chamber.",
       "chamber": "This chamber is a large, empty room with a vault entrance in the center of the floor.",
       ({ "archway", "hallway", }): "A hallway leading north and south is visible on the other side of an archway in the stone walls.",
    ]));
    set_reset(([
        HUMAN_NPC + "greeter.c": 1,
    ]));
}