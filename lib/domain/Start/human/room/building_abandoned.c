#include <human.h>

inherit STD_ROOM;

void create () {
    ::create();
    set_properties(([
        "indoors": 1,
    ]));
    set_short("an abandoned building");
    set_long("The inside of a wide brick building that used to be a warehouse. There is an open double door to the west facing the street letting in a column of light. A collection of old crates are positioned around the warehouse. Dust has built up on the floor, with a cleared path leading off to the side of the room where a staircase leads down to the second floor.");
    set_listen("default", "Several whispers can be heard from some vagrants hanging around.");
    set_smell("default", "A musty dust slowly floats in the air.");
    set_exits(([
        "out northwest": HUMAN_ROOM + "fourth_st_s6.c",
        "down": HUMAN_ROOM + "building_abandoned2.c",
    ]));
    set_looks(([
        ({ "crates", "goods" }): "Old crates positioned around the warehouse are either empty or filled with spoiled goods.",
        "door": "The front of the building consists of a double door that has been opened, providing a column of light to pour upon the dirty floor.",
        "stairs": "The wooden staircase leads down to the darkened basement of the warehouse.",
        "dust": "The dust is an indication of a long passage of time.",
    ]));
    set_reset(([
        HUMAN_NPC + "greasy_rat.c": 1,
    ]));
}