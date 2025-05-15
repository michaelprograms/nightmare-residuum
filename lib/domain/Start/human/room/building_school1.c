#include <human.h>

inherit STD_ROOM;

void create () {
    ::create();
    set_short("a school building");
    set_long("");
    set_exits(([
        "out east": HUMAN_ROOM + "fifth_st_n1.c",
        "out south": HUMAN_ROOM + "market_st_e5.c",
    ]));
    set_looks(([

    ]));
}