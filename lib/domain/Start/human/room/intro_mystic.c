#include "human.h"

inherit STD_ROOM;

void create () {
    ::create();
    set_properties(([
        "no attack": 1,
        "indoors": 1,
    ]));
    set_short("a mystic's hall");
    set_long("The inside of a brick building that consists of a room that is deeper than wide. There is a set of open doors to the north facing the street. Along the walls are stained glass windows consisting of mystics in various combat poses while the ceiling is vaulted high above with exposed wooden beams. Several pews are positioned in rows facing a central altar at the far end of the room. A staircase leads down to the catacombs.");
    set_listen("default", "Chanting of hymns can be heard softly.");
    set_smell("default", "A light floral scent tries to mask the stuffy air.");
    set_exits(([
        "out north": HUMAN_ROOM + "market_st_e5.c",
        "up": HUMAN_ROOM + "bell_tower.c",
        "down": HUMAN_ROOM + "hall_mystic.c",
    ]));
    set_looks(([
        ({ "pews", "altar" }): "Wooden pews used by the mystics in their prayers before an altar of which a senior mystic leads a group session of prayer.",
        ({ "ceiling", "beams" }): "The high ceilings vault overhead with thick exposed wooden beams.",
        "windows": "The windows are spaced along the side walls and reach towards the base of the vaulted ceiling and consist of mystics in several positions of prayer.",
        "doors": "The front of the building consists of a set of doors facing the street without any windows.",
        "stairs": "The wooden staircase leads down to the basement catacombs of the building, where the mystics like to meditate upon their history.",
    ]));
}