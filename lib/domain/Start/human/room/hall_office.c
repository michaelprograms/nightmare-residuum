#include <human.h>

inherit STD_ROOM;

void create () {
    room::create();
    set_properties(([ "indoors": 1, ]));
    set_short("an office hall");
    set_long("The inside of a brick building that consists of a large open hall. The doorway on the west wall is held open by ropes attached to hooks. Desks with chairs are placed in rows.");
    set_exits(([
        "out west": HUMAN_ROOM + "broad_st_n1.c",
    ]));
    set_looks(([
        "building": "The building is set up as an office.",
        "doorway": "The open doorway leads back to the street.",
        ({ "desks", "chairs" }): "The desks each have a chair, and are covered in paperwork.",
        "paperwork": "The paperwork looks to be government related.",
    ]));
}