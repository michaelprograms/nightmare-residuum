#include "human.h"

inherit "/std/room/class_hall.c";

void create () {
    ::create();
    set_properties(([
        "no attack": 1,
    ]));
    set_short("a scoundrel's hall");
    set_long("The basement floor of a warehouse building which happens to be the darkened hideout of the scoundrels. Brick columns form archways to support the warehouse above with dim glowing lights attached to a few. The floor is a dirty compacted stone of the natural terrain. A staircase leads back up to the first floor.");
    set_exits(([
        "up": HUMAN_ROOM + "intro_scoundrel.c",
    ]));
    set_looks(([
        "lights": "The few lights are spaced out, leaving darkened gaps throughout most of the warehouse.",
        "floor": "Earthy rocks compacted by the passage of many feet.",
        "stairs": "The staircase lead back up to the first floor of the warehouse.",
        ({ "columns", "archways" }): "The columns are evenly spaced, with some connecting to form archways, providing support the warehouse floor above.",
    ]));
    set_class("scoundrel");
}
