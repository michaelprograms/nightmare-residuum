#include <human.h>

inherit STD_ROOM;

void create () {
    ::create();
    set_properties(([
        "indoors": 1,
    ]));
    set_short("a rundown row-home");
    set_long("This is a row-home that has seen better days.");
    set_listen("default", "It is quiet in here.");
    set_smell("default", "The house smells musty.");
    set_exits(([
        "out north": HUMAN_ROOM + "walnut_st_e5.c",
    ]));
}