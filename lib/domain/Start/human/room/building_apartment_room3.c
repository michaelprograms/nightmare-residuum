#include <human.h>

inherit STD_ROOM;

void create () {
    ::create();
    set_short("an apartment room");
    set_long("A room in an apartment building.");
    set_exits(([
        "out west": HUMAN_ROOM + "building_apartment2.c",
    ]));
    set_looks(([

    ]));
}