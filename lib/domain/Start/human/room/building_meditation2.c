#include <human.h>

inherit STD_ROOM;

void create () {
    ::create();
    set_properties(([
        "indoors": 1,
    ]));
    set_short("a meditation hall");
    set_long("The second floor of a brick building that consists of the a training area for those seeking to meditate. Mats can be seen scattered around the open floor. A staircase leads back down to the first floor.");
    set_listen("default", "It is extremely quiet and calm.");
    set_smell("default", "A pleasantly neutral odor persists in the building.");
    set_exits(([
        "down": HUMAN_ROOM + "building_meditation.c",
    ]));
    set_looks(([
        "mats": "Mats used to help a psionist meditate.",
        "stairs": "The staircase lead back down to the first floor of the building.",
    ]));
    set_reset(([
        HUMAN_NPC + "teen_meditate.c": 2,
    ]));
}
