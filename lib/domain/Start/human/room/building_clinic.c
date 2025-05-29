#include <human.h>

inherit STD_ROOM;

void create () {
    ::create();
    set_properties(([
        "indoors": 1,
    ]));
    set_short("a village clinic");
    set_long("The inside of a large building which is separated into smaller rooms. The building is used as a clinic, with a reception area in the front by the exit. Offices and smaller rooms for patients are towards the back.");
    set_exits(([
        "out north": HUMAN_ROOM + "pine_st_w1.c",
    ]));
    set_looks(([

    ]));
}