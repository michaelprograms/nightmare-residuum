#include "human.h"

inherit STD_ROOM;

void create () {
    ::create();
    set_properties(([
        "no attack": 1,
        "indoors": 1,
    ]));
    set_short("a ranger's hall");
    set_long("The middle of a field of grasses and wildflowers that consists of the training area of the rangers. A brick-front can be seen to the east. Several archery targets are placed a distance away for rangers to practice upon. A wooden fence surrounds the field to obscure view.");
    set_listen("default", "Quiet interspersed with several thuds of arrows hitting targets.");
    set_smell("default", "The refreshing scent of a wide open green space and wildflowers.");
    set_exits(([
        "east": HUMAN_ROOM + "intro_ranger.c",
    ]));
    set_looks(([
        ({ "building", "brick-front", "brick front", "brick", "archway", "gate", "entrance" }): "The brick-front and gated archway can be seen to the east, blocking entrance to the street.",
        "field": "A field full of an assortment of grasses with some wildflowers mixed in. Archery targets are placed out in the field.",
        ({ "archery", "targets" }): "Targets upon which archers can shoot arrows are setup in the field.",
        "fence": "The fence stretches out and boxes in the field from outside view.",
    ]));
}
