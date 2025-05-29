#include <human.h>

inherit STD_ROOM;

void create () {
    ::create();
    set_properties(([
        "indoors": 1,
    ]));
    set_short("a village clinic");
    set_long("The small patient room in the back of a hospital clinic. A door leads back into the main office to the north.");
    set_exits(([
        "north": HUMAN_ROOM + "building_clinic1.c",
    ]));
    set_looks(([

    ]));
    set_reset(([
        HUMAN_NPC + "clinic_nurse.c": 1,
        HUMAN_NPC + "clinic_patient.c": 1,
    ]));
}