#include "human.h"

inherit STD_ROOM;

void create () {
    ::create();
    set_properties(([
        "no attack": 1,
        "indoors": 1,
    ]));
    set_short("a village cafe");
    set_long("The inside of a brick building set up as a small cafe with a few tables and chairs. There are a large glass windows facing the streets to the west and east with doors alongside them. A counter along the southern wall has a sign above that lists the available drinks.");
    set_exits(([
        "out east": HUMAN_ROOM + "parkway_2.c",
        "out west": HUMAN_ROOM + "second_st_n2.c",
    ]));
    set_looks(([

    ]));
    set_reset(([
        HUMAN_NPC + "vendor_cafe.c": 1,
    ]));
}