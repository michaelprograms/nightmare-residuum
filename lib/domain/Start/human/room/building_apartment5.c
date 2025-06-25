#include <human.h>

inherit STD_ROOM;

void create () {
    ::create();
    set_short("an apartment building");
    set_long("A hallway floor of an apartment building. It is narrow, only allowing enough space for a few people to traverse. The floor is made of wooden planks that have been smoothed by traffic. Brick walls frame the hallway and entryways. The hallway continues west and south.");
    set_exits(([
        "west": HUMAN_ROOM + "building_apartment4.c",
        "south": HUMAN_ROOM + "building_apartment6.c",
        "enter north": HUMAN_ROOM + "building_apartment_room6.c",
        "enter east": HUMAN_ROOM + "building_apartment_room7.c",
    ]));
    set_looks(([
        "floor": "The floor has been smoothed by the passage of many people.",
        "building": "The building houses many people.",
        "walls": "The exposed brick walls are reminiscent of many of the buildings in this village.",
    ]));
}