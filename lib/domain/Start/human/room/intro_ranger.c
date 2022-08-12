#include "human.h"

inherit ROOM_CLASS_INTRO;

void create () {
    ::create();
    set_properties(([
        "no attack": 1,
    ]));
    set_short("a ranger's hall");
    set_long("The inside of a wide brick building which isn't much since it ends quickly after beginning. A field of mixed grasses stretches to the west behind the brick-front. The entrance is an archway with an open metal gate separating the inside from the street. Several archery targets are placed to the west out in the field. A wooden fence surrounds the field to obscure view.");
    set_listen("default", "Quiet interspersed with several thuds of arrows hitting targets.");
    set_smell("default", "Old masonry mixed with the refreshing scent of wildflowers.");
    set_exits(([
        "out east": HUMAN_ROOM + "pine_st_n6.c",
    ]));
    set_exit("west", HUMAN_ROOM + "hall_ranger.c", (: handle_pre_exit :), 0);
    set_looks(([
        "building": "Beyond the brick-front and gated archway, there really isn't much more to the building. Is it really a building?",
        "field": "A field full of an assortment of grasses with some wildflowers mixed in. Archery targets are placed out in the field.",
        ({ "entrance", "archway", "gate" }): "The gated archway is the entrance to this building, which houses the field.",
        "street": "The street is particially obscured by the gate in the archway but some of it is still visible. It looks quiet out there.",
        ({ "archery", "targets" }): "Targets upon which archers can shoot arrows are setup in the field to the west.",
        "fence": "The fence stretches out to the west and boxes in the field from outside view.",
    ]));
    set_class("ranger");
}