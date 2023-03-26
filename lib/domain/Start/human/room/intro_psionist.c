#include "human.h"

inherit ROOM_CLASS_INTRO;

void create () {
    ::create();
    set_properties(([
        "no attack": 1,
        "indoors": 1,
    ]));
    set_short("a psionist's hall");
    set_long("The inside of a wide brick building that consists of a large unfurnished room. There is an open door to the east facing the street next to several wide and tall windows. Meditation mats are scattered around the floor. A staircase leads up to the second floor.");
    set_listen("default", "It is extremely quiet and calm.");
    set_smell("default", "A pleasantly neutral odor persists in the building.");
    set_exits(([
        "out east": HUMAN_ROOM + "second_st_n1.c",
    ]));
    set_exit("up", HUMAN_ROOM + "hall_psionist.c", (: handle_pre_exit :), 0);
    set_looks(([
        ({ "mats" }): "Small mats used by those attempting to meditate, despite your intrusion.",
        ({ "door", "window" }): "The front of the building consists of a door that swings open to the street and several wide and tall windows.",
        "stairs": "The staircase lead up to the second floor of the building, where the private training of psionists takes place.",
    ]));
    set_class("psionist");
}