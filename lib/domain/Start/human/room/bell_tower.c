#include <human.h>

inherit STD_ROOM;

void create () {
    ::create();
    set_short("a bell tower above a village");
    set_long("A tower above a vaulted roof of a temple. The wooden boards provided a small platform to walk around a large bell hung in the center. The sides of the tower are open. The village street can be seen below. A framework of hexagons glimmer overhead.");
    set_listen("default", "The whispering chants of hymns can be heard below.");
    set_smell("default", "A breeze of freshened air flows past.");
    set_exits(([
        "down": HUMAN_ROOM + "building_church.c",
    ]));
    set_looks(([
        "tower": "The tower is above the temple and contains a bell. The sides of the tower are open to the air to allow the bell's ringing to carry unimpeded.",
        "street": "The quiet village street passes before the temple.",
        "bell": "A bell hangs from the center of the tower, with another piece of metal for a clapper dangling from inside the bell.",
        "platform": "A creaky wooden platform built into the tower to allow a person passage to operate the bell tower.",
        ({ "framework", "hexagons"}): "Far above, the hexagons form a geodesic dome.",
    ]));
}