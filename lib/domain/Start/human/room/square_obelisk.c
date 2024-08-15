#include <human.h>

inherit STD_ROOM;

void create () {
    ::create();
    set_properties(([
        "no attack": 1,
    ]));
    set_short("inside an obelisk");
    set_long("The interior stone walls of the obelisk provide a cool respite.");
    set_listen("default", "The square outside is muffled from within here.");
    set_exits(([
        "out": HUMAN_ROOM + "square.c",
    ]));
    set_looks(([

    ]));
    set_reset(([

    ]));
}