#include <human.h>

inherit STD_ROOM;

void create () {
    ::create();
    set_properties(([
        "indoors": 1,
    ]));
    set_short("a village restaurant");
    set_long("The inside of a brick building set up as a small restaurant with a few tables and chairs. There are a large glass windows facing the streets to the southeast with doors alongside them. A counter along the northwestern wall stands in front of kitchen area.");
    set_exits(([
        "out southeast": HUMAN_ROOM + "park_se.c",
    ]));
}
