#include <human.h>

inherit STD_ROOM;

void create () {
    ::create();
    set_properties(([
        "indoors": 1,
    ]));
    set_short("a cozy row-home");
    set_long("This is a charming row-home with a warm and inviting atmosphere. "
        "The walls are painted in soft, neutral tones, and the wood floors "
        "gleam under the light of a modest chandelier hanging from the "
        "ceiling. A comfortable sofa and a few armchairs are arranged around "
        "a small table, creating a cozy seating area. The large front window "
        "lets in plenty of natural light, and a few potted plants add a touch "
        "of greenery to the space.");
    set_listen("default", "The occasional chirping of birds outside.");
    set_smell("default", "The air smells faintly of freshly brewed tea.");
    set_exits(([
        "out north": HUMAN_ROOM + "walnut_st_w5.c",
        "up": HUMAN_ROOM + "building_nice2b.c",
    ]));
    set_looks(([
        "sofa": "A plush, comfortable sofa with soft cushions, perfect for relaxing.",
        "chandelier": "A modest chandelier that casts a warm glow over the room.",
        "plants": "A few potted plants, including a fern and a small ficus, add a touch of life to the space.",
        "window": "A large front window with sheer curtains that let in plenty of natural light.",
    ]));
    set_reset(([
        HUMAN_NPC + "adult_preppy.c": 1,
    ]));
}