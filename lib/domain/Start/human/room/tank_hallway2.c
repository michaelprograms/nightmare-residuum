#include "human.h"

inherit STD_ROOM;

void create () {
    ::create();
    set_properties(([ "indoors": 1, ]));
    set_short("a hallway junction");
    set_long("A sparse hallway junction in front of an open tank. A large number two is etched onto the wall next to the tank. The floor is covered in dust with footprints leading to the east. A hallway leading west to east walks in front of several tanks, while a shorter hallway leads north away from the tanks.");
    set_exits(([
        "west": HUMAN_ROOM + "tank_hallway1.c",
        "east": HUMAN_ROOM + "tank_hallway3.c",
        "north": HUMAN_ROOM + "tank_exit.c",
    ]));
    set_looks(([
        "hallway": "A very utilitarian hallway, simple in design but built to last.",
        "junction": "Two hallways meet here.",
        "tank": "It appears to be a chamber for cloning a living organism.",
        "wall": "The walls are an uncannily uniform grey color that show no signs of texture or age.",
        "floor": "The floor is covered with dust from the passage a large amount of time, though footprints look to have disturbed it occasionally.",
        "footprint": "The footprints lead away from the tank.",
    ]));
}