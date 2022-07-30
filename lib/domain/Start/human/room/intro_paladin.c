#include "human.h"

inherit "/std/room/class_intro.c";

void create () {
    ::create();
    set_properties(([
        "no attack": 1,
    ]));
    set_short("a paladin's hall");
    set_long("The inside of a wide brick building that consists of a tall, ornate room. There is an open door to the east facing the street. Along the walls are stained glass windows consisting of paladins in various combat poses while the ceiling is arched high above. Several pews are positioned before stations that are used for prayer. A staircase leads down to the second floor.");
    set_listen("default", "Several paladins occasionally break into chant from the pews.");
    set_smell("default", "The fragrance of incense lightly hangs in the air.");
    set_exits(([
        "out east": HUMAN_ROOM + "spruce_st_n1.c",
    ]));
    set_exit("down", HUMAN_ROOM + "hall_paladin.c", (: handle_pre_exit :), 0);
    set_looks(([
        ({ "pews", "stations" }): "Wooden pews used by the paladins to conduct prayer, along with stations used by a paladin to lead others through prayer.",
        "ceiling": "The high ceilings arch above with strong wooden beams holding it the roof in place.",
        "windows": "The windows are spaced along the side walls and reach towards the base of the arched ceiling and consist of paladins posed in various positions.",
        "door": "The front of the building consists of a door that swings open to the street, but no window.",
        "stairs": "The wooden staircase leads down to the basement of the building, where the paladins like to dwell to remind them of their duty.",
    ]));
    set_class("paladin");
}