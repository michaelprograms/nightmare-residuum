#include <human.h>

inherit STD_ROOM;

void create () {
    ::create();
    set_properties(([
        "indoors": 1,
    ]));
    set_short("a village clinic");
    set_long("The inside of a large building used as a clinic, with a reception area in the front by the exit. A smaller room for patients is towards the back.");
    set_exits(([
        "out north": HUMAN_ROOM + "pine_st_w1.c",
        "south": HUMAN_ROOM + "building_clinic2.c",
    ]));
    set_looks(([

    ]));
    set_reset(([
        HUMAN_NPC + "clinic_receptionist.c": 1,
        HUMAN_NPC + "clinic_patient.c": 1,
    ]));
}