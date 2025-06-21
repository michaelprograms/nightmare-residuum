#include <human.h>

inherit STD_ROOM;

void create () {
    ::create();
    set_short("an apartment building");
    set_long("An apartment building.");
    set_exits(([
        "west": HUMAN_ROOM + "building_apartment1.c",
        "south": HUMAN_ROOM + "building_apartment3.c",
    ]));
    set_looks(([

    ]));
}