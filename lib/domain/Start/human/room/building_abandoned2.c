#include <human.h>

inherit STD_ROOM;

void create () {
    ::create();
    set_properties(([
        "indoors": 1,
    ]));
    set_short("an abandoned building");
    set_long("The basement floor of a warehouse building. Brick columns form archways to support the warehouse above with dim glowing lights attached to a few. The floor is a dirty compacted stone of the natural terrain. A staircase leads back up to the first floor.");
    set_listen("default", "Quiet is occasionally broken by a drip into a puddle.");
    set_smell("default", "A damp earthy smell overpowers the musty dust.");
    set_exits(([
        "up": HUMAN_ROOM + "building_abandoned.c",
    ]));
    set_looks(([
        "lights": "The few lights are spaced out, leaving darkened gaps throughout most of the warehouse.",
        "floor": "Earthy rocks compacted by the passage of many feet.",
        "stairs": "The staircase lead back up to the first floor of the warehouse.",
        ({ "columns", "archways" }): "The columns are evenly spaced, with some connecting to form archways, providing support the warehouse floor above.",
    ]));
}
