#include <human.h>

inherit STD_ROOM;

void create () {
    ::create();
    set_properties(([
        "no attack": 1,
        "indoors": 1,
    ]));
    set_exits(([
        "out southeast": HUMAN_ROOM + "park_se.c",
    ]));
}
