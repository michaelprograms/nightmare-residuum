#include <human.h>

inherit STD_ROOM;

void create () {
    ::create();
    set_properties(([
        "indoors": 1,
    ]));
    set_short("a brewery");
    set_long("The inside of a wide brick building filled with a business that brews beer. Large metal vats line the walls as they ferment several kinds of beer. Wooden barrels are stacked in one corner, and a few tools hang from hooks near a sturdy workbench.");
    set_listen("default", "The soft sound of bubbles come from the vats.");
    set_smell("default", "The warm, yeasty scent of beer.");
    set_exits(([
        "out south": HUMAN_ROOM + "spruce_st_e2.c",
    ]));
    set_looks(([
    "vats": "Large metal vats, each filled with beer at different stages of fermentation. Bubbles rise to the surface inside.",
    "barrels": "Sturdy wooden barrels are stacked neatly in the corner, ready to be filled with fresh beer.",
    "tools": "Various brewing tools, such as ladles, tongs, and brushes, hang from hooks near the workbench.",
    "workbench": "A sturdy wooden workbench stained from years of use. It holds a few scattered brewing notes and empty mugs.",
]));
}