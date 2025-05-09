#include <human.h>

inherit STD_ROOM;

void create () {
    ::create();
    set_properties(([
        "indoors": 1,
    ]));
    set_short("a temple hall");
    set_long("The inside of a wide brick building that consists of a tall, ornate room. There is an open door to the east facing the street. Along the walls are stained glass windows consisting of figures in various poses while the ceiling is arched high above. Several pews are positioned before stations that are used for prayer. A staircase leads down to the basement.");
    set_listen("default", "Chants occasionally break in from the pews.");
    set_smell("default", "The fragrance of incense lightly hangs in the air.");
    set_exits(([
        "out east": HUMAN_ROOM + "fourth_st_n1.c",
        "down": HUMAN_ROOM + "building_temple2.c",
    ]));
    set_looks(([
        ({ "pews", "stations" }): "Wooden pews with which to conduct prayer, along with stations used to lead others through prayer.",
        "ceiling": "The high ceilings arch above with strong wooden beams holding it the roof in place.",
        "windows": "The windows are spaced along the side walls and reach towards the base of the arched ceiling. They consist of figures pictured in various positions.",
        "door": "The front of the building consists of a door that swings open to the street, but no window.",
        "stairs": "The wooden staircase leads down to the basement of the building.",
    ]));
}