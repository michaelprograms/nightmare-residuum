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
        "out west": HUMAN_ROOM + "fifth_st_s2.c",
    ]));
    set_reset(([
        HUMAN_NPC + "adult_suspicious.c": 1,
    ]));
}