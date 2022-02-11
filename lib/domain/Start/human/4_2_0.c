inherit STD_ROOM;

void create () {
    ::create();
    set_short("a grassy meadow");
    set_long("A meadow filled with wild grasses and flowers. The west side of the meadow abuts an enormous stone structure with a wide opening at its base. A framework of hexagons glimmer far overhead.");
    set_looks(([
        ({ "grasses", "flowers" }): "A simple variety of wild grasses and flowers.",
        "structure": "A large rock formation jutting out of the ground.",
        "hexagons": "Far above, the hexagons form a geodesic dome.",
    ]));
    set_exits(([
        "enter west": "/domain/Start/human/4_2_0_enter.c",
        "southeast": "/domain/Start/human/5_3_0.c",
    ]));
}