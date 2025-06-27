#include <human.h>

inherit STD_ROOM;

void create () {
    ::create();
    set_short("an apartment room");
    set_long("This is a small studio apartment with a bed in one corner and a compact kitchen along one wall. A table and chair sit near a window.");
    set_exits(([
        "out south": HUMAN_ROOM + "building_apartment1.c",
    ]));
    set_looks(([
        "bed": "A simple bed with a plain blanket, neatly made.",
        "kitchen": "A compact kitchen area with a small stove and a few cabinets.",
        "table": "A small table with a single chair, placed near the window.",
        "window": "A window that lets in some natural light, offering a view of the street outside.",
    ]));
}