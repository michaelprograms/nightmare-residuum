#include <human.h>

inherit STD_ROOM;

void create () {
    ::create();
    set_short("a school building");
    set_long("A hallway in a simple school building with scuffed floors from "
        "the passage of many feet. To the west and north, wooden doors lead "
        "into classrooms. Double doors lead out onto the city streets to the "
        "east and south.");
    set_exits(([
        "west": HUMAN_ROOM + "building_school2.c",
        "north": HUMAN_ROOM + "building_school3.c",
        "out east": HUMAN_ROOM + "fifth_st_n1.c",
        "out south": HUMAN_ROOM + "market_st_e5.c",
    ]));
    set_looks(([

    ]));
}