#include <human.h>

inherit STD_ROOM;

void create () {
    ::create();
    set_properties(([
        "indoors": 1,
    ]));
    set_short("a cozy upstairs bedroom");
    set_long("This is a cozy upstairs bedroom with a relaxing atmosphere. "
        "The walls are painted in soft tones, and a rug covers most of the "
        "hardwood floor. A neatly made queen-sized bed sits against one "
        "wall, flanked by nightstands with small lamps. A dresser stands "
        "in the corner and an armchair is tucked near the window. Two windows "
        "facing the street let in natural light.");
    set_listen("default", "Relative quiet.");
    set_smell("default", "Fresh linens from the bed.");
    set_exits(([
        "down": HUMAN_ROOM + "building_nice2a.c",
    ]));
    set_looks(([
        "bed": "A queen-sized bed with fresh sheets.",
        "rug": "The rug covers the wooden floor and really brings the room together.",
        "nightstands": "Matching nightstands with small lamps and a few personal items.",
        "dresser": "A tall wooden dresser with polished handles, perfect for storing clothes.",
        "armchair": "A small, comfortable armchair positioned near a window, ideal for reading.",
        "windows": "Two windows on the front of the house that let in plenty of natural light.",
    ]));
    set_reset(([
        HUMAN_NPC + "adult_preppy.c": 1,
    ]));
}