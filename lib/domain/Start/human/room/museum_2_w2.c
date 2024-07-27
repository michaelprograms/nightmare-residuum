#include <human.h>

inherit STD_ROOM;

void create () {
    ::create();
    set_properties(([ "indoors": 1, "no attack": 1, ]));
    set_short("a historical building");
    set_long("The western hallway of a building with high ceilings and polished floors. The walls are made of stone blocks and covered with tapestries, except for the southern wall which is covered in a painting. The end of the hallway comes to a staircase leading up to the second floor.");
    set_exits(([
        "east": HUMAN_ROOM + "museum_2_w1.c",
        "down": HUMAN_ROOM + "museum_1_w2.c",
    ]));
    set_looks(([
        ({ "building", "ceiling", "walls" }): "A building of large proportions. This hallway is a large room with high ceilings and distant walls.",
        "floor": "The floor is a polished white stone in large blocks.",
        "hallway": "It leads away and also towards the lobby of the building.",
        "wall" : "A large painting covers nearly all of the southern wall.",
    ]));
    set_reset(([
        HUMAN_ITEM + "painting_4.c": 1,
    ]));
}