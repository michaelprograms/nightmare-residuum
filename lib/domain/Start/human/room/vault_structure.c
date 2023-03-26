#include "human.h"

inherit STD_ROOM;

void create () {
    ::create();
    set_properties(([ "indoors": 1, ]));
    set_short("a large stone structure");
    set_long("The interior cavity of a tall structure which grows narrower towards the top. Four stone walls enclose the chamber at a distance. Several lamps are scattered around the cavern, casting a white light across the room. Doorways can be seen on the northern and southern walls.");
    set_looks(([
        ({ "structure", "cavity" }): "Inside of a tall stone structure which is narrower at the top. Far above the top of the structure turns to darkness before a ceiling is seen.",
        "wall": "Dimly lit stone walls in the distance enclose the chamber.",
        "lamps": "The standing lamps brighten the center of the chamber substantially, but leave the walls dim in the distance.",
        "doorways": "There are identical doorways on the northern and southern walls, however there is a down arrow etched into the southern wall while the northern wall has an up arrow.",
    ]));
    set_exits(([
        "enter north": HUMAN_ROOM + "vault_elevator.c",
    ]));
    set_reset(([
        HUMAN_NPC + "rat.c": 1,
    ]));
}