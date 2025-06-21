#include <human.h>

inherit STD_ROOM;

void create () {
    ::create();
    set_short("an apartment building");
    set_long("An apartment building.");
    set_exits(([
        "out west": HUMAN_ROOM + "chestnut_st_e6.c",
        "north": HUMAN_ROOM + "building_apartment2.c",
    ]));
    set_looks(([

    ]));
}