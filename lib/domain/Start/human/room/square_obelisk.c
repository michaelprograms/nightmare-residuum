#include <human.h>

inherit STD_ROOM;

void create () {
    ::create();
    set_properties(([
        "no attack": 1,
    ]));
    set_short("inside an obelisk");
    set_long("The interior stone walls of the obelisk provide a cool respite. There are openings on each wall leading back outside. There is no ceiling, the walls tower overhead as they narrow and form the tip of the obelisk.");
    set_listen("default", "The square outside is muffled from within here.");
    set_exits(([
        "out west": HUMAN_ROOM + "square.c",
        "out north": HUMAN_ROOM + "square.c",
        "out east": HUMAN_ROOM + "square.c",
        "out south": HUMAN_ROOM + "square.c",
    ]));
    set_looks(([

    ]));
    set_reset(([
        HUMAN_NPC + "mayor.c": 1,
    ]));
}