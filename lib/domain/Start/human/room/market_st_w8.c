#include "human.h"

inherit STD_ROOM;

void create () {
    ::create();
    set_properties(([ "indoors": 1, ]));
    set_short("inside a concrete structure");
    set_long("A cramped structure made of concrete. A hatch on the east wall allows entry inside of the dome, while another hatch on the west wall leading out of the dome. Light from the sky is funneled through skylights on the outside of the dome.");
    set_exits(([
        "out east": HUMAN_ROOM + "market_st_w7.c",
        "out west": HUMAN_ROOM + "market_st_w9.c",
    ]));
    set_looks(([
        "structure": "A small enclosed space within the dome structure. It allows entry into and out of the dome.",
        "hatch": "There are two hatches here, one to the west and one to the east, allowing entry into and out of the dome.",
        "light": "The light is redirected with reflective surfaces to provide natural light in here.",
    ]));
    set_reset(([
        HUMAN_MONSTER + "plasma_snail.c": 2,
        HUMAN_NPC + "guard_clone.c": 1,
    ]));
}