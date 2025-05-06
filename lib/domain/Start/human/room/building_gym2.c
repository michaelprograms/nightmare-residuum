#include <human.h>

inherit STD_ROOM;

void create () {
    ::create();
    set_properties(([
        "indoors": 1,
    ]));
    set_short("a gym hall");
    set_long("The second floor of a brick building that consists of the personal training area. Training equipment can be seen scattered around the sides of the room. A staircase leads back down to the first floor.");
    set_listen("default", "Several people talk boisterously while lifting weights.");
    set_smell("default", "The warm smell of training bodies.");
    set_exits(([
        "down": HUMAN_ROOM + "building_gym1.c",
    ]));
    set_looks(([
        "equipment": "Things used to make a warrior strong.",
        "stairs": "The staircase lead back down to the first floor of the building.",
    ]));
}
