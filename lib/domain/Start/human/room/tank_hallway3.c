#include "human.h"

inherit STD_ROOM;

void create () {
    ::create();
    set_short("a hallway");
    set_long("The end of a sparse hallway in front of an open tank. A large number three is etched onto the wall next to the tank. The floor is covered in dust with footprints leading to the west.");
    set_exits(([
        "west": HUMAN_ROOM + "tank_hallway2.c",
    ]));
    set_looks(([
        "hallway": "A very utilitarian hallway, simple in design but built to last.",
        "tank": "It appears to be a chamber for cloning a living organism.",
        "wall": "The walls are an uncannily uniform grey color that show no signs of texture or age.",
        "floor": "The floor is covered with dust from the passage a large amount of time, though footprints look to have disturbed it occasionally.",
        "footprint": "The footprints lead away from the tank.",
    ]));
}