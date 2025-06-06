#include <human.h>

inherit STD_ROOM;

void create () {
    ::create();
    set_properties(([
        "indoors": 1,
    ]));
    set_short("an archery hall");
    set_long("The inside of a wide brick building which isn't very large since it ends quickly after beginning. A field of mixed grasses stretches to the west behind the brick-front. The entrance is an archway with an open metal gate separating the inside from the street. Several archery targets are placed to the west out in the field. A wooden fence surrounds the field to obscure view.");
    set_listen("default", "Quiet interspersed with the occasional thuds of arrows hitting targets.");
    set_smell("default", "Old masonry mixed with the refreshing scent of wildflowers.");
    set_exits(([
        "out east": HUMAN_ROOM + "second_st_n6.c",
        "west": HUMAN_ROOM + "archery_range.c",
    ]));
    set_looks(([
        "building": "Beyond the brick-front and gated archway, there really isn't much more to the building. Is it really a building?",
        "field": "A field full of an assortment of grasses with some wildflowers mixed in. Archery targets are placed out in the field.",
        ({ "entrance", "archway", "gate" }): "The gated archway is the entrance to this building, which houses the field.",
        "street": "The street is partially obscured by the gate in the archway but some of it is still visible. It looks quiet out there.",
        ({ "archery", "targets" }): "Targets upon which archers can shoot arrows are setup in the field to the west.",
        "fence": "The fence stretches out to the west and boxes in the field from outside view.",
    ]));
}