#include <human.h>

inherit STD_ROOM;

void create () {
    ::create();
    set_short("an apartment building");
    set_long("A lobby floor of an apartment building. It is spacious enough to allow for foot traffic of numerous people. The floor is made of wooden planks that have been smoothed by traffic. Brick walls frame the hallway and entryways. The lobby continues east.");
    set_exits(([
        "out south": HUMAN_ROOM + "chestnut_st_e6.c",
        "east": HUMAN_ROOM + "building_apartment2.c",
        "up": HUMAN_ROOM + "building_apartment4.c",
    ]));
    set_looks(([
        "floor": "The floor has been smoothed by the passage of many people.",
        "building": "The building houses many people.",
        "walls": "The exposed brick walls are reminiscent of many of the buildings in this village.",
    ]));
}