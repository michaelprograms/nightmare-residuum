#include <human.h>

inherit STD_ROOM;

void create () {
    ::create();
    set_properties(([
        "indoors": 1,
    ]));
    set_short("a typical row-home");
    set_long("This is a typical row-home with a simple and functional design. "
        "The walls are painted in neutral colors, and the floors show signs "
        "of wear but are well-maintained. A modest sofa and a "
        "couple of chairs are arranged around a coffee table in the living "
        "area. The room is lit by a ceiling light fixture, and a small window "
        "near the front door lets in some natural light.");
    set_listen("default", "You hear the faint sounds of the neighborhood outside.");
    set_smell("default", "The air smells faintly of cleaning products and wood polish.");
    set_exits(([
        "out west": HUMAN_ROOM + "fifth_st_n2.c",
    ]));
}