#include "human.h"

inherit STD_ROOM;

void create () {
    ::create();
    set_properties(([
        "no attack": 1,
        "indoors": 1,
    ]));
    set_short("a psionist's hall");
    set_long("The second floor of a brick building that consists of the personal training area of the psionists. Meditation mats can be seen scattered around the open floor. A staircase leads back down to the first floor.");
    set_listen("default", "It is extremely quiet and calm.");
    set_smell("default", "A pleasantly neutral odor persists in the building.");
    set_exits(([
        "down": HUMAN_ROOM + "intro_psionist.c",
    ]));
    set_looks(([
        "mats": "Mats used to help a psionist meditate.",
        "stairs": "The staircase lead back down to the first floor of the building.",
    ]));
}
