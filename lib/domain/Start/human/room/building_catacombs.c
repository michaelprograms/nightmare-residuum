#include <human.h>

inherit STD_ROOM;

void create () {
    ::create();
    set_properties(([
        "indoors": 1,
    ]));
    set_short("a catacombs");
    set_long("The basement floor of a brick building that consists of the catacombs of the church. The walls are filled with chambers upon which dried remains rest. The ground is a mixture of gravel and dirt. A staircase leads back up to the first floor.");
    set_listen("default", "The gravel and dirt mixture crunches lightly underfoot.");
    set_smell("default", "Stale perfume hangs in the humid air.");
    set_exits(([
        "up": HUMAN_ROOM + "building_church.c",
    ]));
    set_looks(([
        "floor": "The floor is a mixture of broken stones forming gravel and dirt.",
        "stairs": "The staircase lead back up to the first floor of the temple.",
        ({ "walls", "chambers", "remains" }): "Important people resting after their duties have completed.",
    ]));
}
