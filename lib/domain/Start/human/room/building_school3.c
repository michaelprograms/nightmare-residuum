#include <human.h>

inherit STD_ROOM;

void create () {
    ::create();
    set_short("a school classroom");
    set_long("A classroom in a simple school building with scuffed floors from "
        "the passage of many feet. Desks and chairs face a chalkboard and a "
        "teacher's desk. An open door leads east back to the main hallway.");
    set_exits(([
        "south": HUMAN_ROOM + "building_school1.c",
    ]));
    set_looks(([
        ({ "hallway", "door" }): "A door leads to the hallway which connects "
            "several classrooms and leads outside.",
        "floors": "The floors are scuffed from travel.",
        "desks": "Student desks facing the front of the classroom where a "
            "teacher's desk faces them.",
        "chalkboard": "The chalkboard contains some elementary math.",
    ]));
    set_reset(([
        HUMAN_NPC + "adult_teacher.c": 1,
        HUMAN_NPC + "teen_student.c": 4,
    ]));
}