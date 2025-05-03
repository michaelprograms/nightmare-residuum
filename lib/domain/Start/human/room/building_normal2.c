#include <human.h>

inherit STD_ROOM;

void create () {
    ::create();
    set_properties(([
        "indoors": 1,
    ]));
    set_short("a simple row-home");
    set_long("This is a simple row-home with a functional design. The walls "
        "are painted in neutral colors, and the floors show signs of wear "
        "but are well-maintained. A simple sofa and a couple of chairs are "
        "arranged around a table in the living area. The room is lit by a "
        "ceiling light fixture, and a small window near the front door lets "
        "in some light.");
    set_listen("default", "You hear the faint sounds of the neighborhood outside.");
    set_smell("default", "The air smells faintly of cleaning products and wood polish.");
    set_exits(([
        "out south": HUMAN_ROOM + "chestnut_st_e5.c",
    ]));
    set_reset(([
        HUMAN_NPC + "adult_normal.c": 1,
    ]));
}